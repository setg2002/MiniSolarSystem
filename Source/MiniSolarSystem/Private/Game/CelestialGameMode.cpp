// Copyright Soren Gilbertson

#include "Game/CelestialGameMode.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Helpers/BodySystemFunctionLibrary.h"
#include "Color/ColorCurveFunctionLibrary.h"
#include "Game/CelestialSaveGameArchive.h"
#include "Color/GasGiantColorSettings.h"
#include "Game/CelestialGameInstance.h"
#include "Bodies/Components/RingSystemComponent.h"
#include "Bodies/Components/AtmosphereComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Game/CelestialSaveGame.h"
#include "NiagaraComponent.h"
#include "OrbitDebugActor.h"
#include "Bodies/CelestialObject.h"
#include "Player/CelestialPlayer.h"
#include "Player/OverviewPlayer.h"
#include "Color/ColorSettings.h"
#include "Shape/ShapeSettings.h"
#include "Bodies/CelestialBody.h"
#include "Shape/NoiseSettings.h"
#include "NiagaraActor.h"
#include "EngineUtils.h"
#include "Shape/NoiseLayer.h"
#include "Bodies/GasGiant.h"
#include "Bodies/Planet.h"
#include "Bodies/Star.h"
#include "Blueprint/UserWidget.h"
#include "Game/CelestialGameState.h"
#include "Materials/MaterialParameterCollection.h"


float ACelestialGameMode::gravitationalConstant;

ACelestialGameMode::ACelestialGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	currentPerspective = 255;
	gravitationalConstant = 100;
	
	GameStateClass = ACelestialGameState::StaticClass();
}

void ACelestialGameMode::AddBodySystem(FBodySystem& NewSystem)
{
	BodySystems.Add(NewSystem);
}

void ACelestialGameMode::RemoveBodySystem(FBodySystem& System)
{
	BodySystems.Remove(System);
}

void ACelestialGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlanetIlluminationInst = GetWorld()->GetParameterCollectionInstance(LoadObject<UMaterialParameterCollection>(NULL, TEXT("MaterialParameterCollection'/Game/Materials/PlanetIllumination.PlanetIllumination'"), NULL, LOAD_None, NULL));

	PC = GetWorld()->GetFirstPlayerController();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// Make widgets
	CelestialWidget = CreateWidget<UUserWidget, APlayerController*>(GetWorld()->GetFirstPlayerController(), CelestialWidgetClass);
	OverviewWidget  = CreateWidget<UUserWidget, APlayerController*>(GetWorld()->GetFirstPlayerController(), OverviewWidgetClass);
	PauseWidget = CreateWidget<UUserWidget, APlayerController*>(GetWorld()->GetFirstPlayerController(), PauseWidgetClass);

	CelestialPlayer = Cast<ACelestialPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ACelestialPlayer::StaticClass()));
	OverviewPlayer = Cast<AOverviewPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOverviewPlayer::StaticClass()));

	CelestialPlayer->SetWidget(CelestialWidget);

	// Gets all ACelestialBodies and adds them to bodies
	for (TActorIterator<ACelestialBody> Itr(GetWorld()); Itr; ++Itr) {
		Bodies.Add(*Itr);

		if (Cast<AStar>(*Itr))
		{
			AStar* Star = Cast<AStar>(*Itr);
			Star->SetStarNum(Stars.Num());
			Stars.Add(Star);
		}
	}
	PlanetIlluminationInst->SetScalarParameterValue("NumStars", Stars.Num());

	// Gets all actors that implement ICelestialObject and adds them to celestialObjects
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GWorld, UCelestialObject::StaticClass(), Actors);
	for (auto& actor : Actors)
	{
		const auto &Interface = Cast<ICelestialObject>(actor);
		CelestialObjects.Add(Interface);
	}
	
	
	for (TActorIterator<ANiagaraActor> Itr(GetWorld()); Itr; ++Itr)
	{
		Asteroids.AddUnique(*Itr);
	}

	LoadGame();
}

void ACelestialGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentPerspective == 1)
	{
		for (int32 i = 0; i < CelestialObjects.Num(); i++) 
		{
			ICelestialObject* thisObject = CelestialObjects[i];

			thisObject->UpdateVelocity(Bodies, DeltaTime);
			thisObject->UpdatePosition(DeltaTime);
		}
	}

	// Set the gravity location of asteroid Niagara systems to the location of the most massive body
	ACelestialBody* LargestBody = nullptr;
	for (auto& Body : Bodies)
	{
		if (LargestBody == nullptr)
			LargestBody = Body;
		else if (Body->GetMass() > LargestBody->GetMass())
			LargestBody = Body;
	}
	
	for (auto& System : Asteroids)
	{
		System->GetNiagaraComponent()->SetVariableVec3(FName("GravityPos"), LargestBody->GetActorLocation());
	}
}

ACelestialBody* ACelestialGameMode::AddBody(TSubclassOf<ACelestialBody> Class, FName Name, FTransform Transform, bool bRegenerate)
{
	ACelestialBody* NewBody = GetWorld()->SpawnActor<ACelestialBody>(Class, Transform);
	NewBody->SetName(Name);
	NewBody->SetMass(1);

	Bodies.Add(NewBody);

	if (AStar* Star = Cast<AStar>(NewBody))
	{
		Star->SetStarNum(Stars.Num());
		Stars.Add(Star);
		PlanetIlluminationInst->SetScalarParameterValue("NumStars", Stars.Num());
	}
	else if (bRegenerate)
	{
		if (APlanet* Planet = Cast<APlanet>(NewBody))
			Planet->ReGenerate();
	}

	const auto &Interface = Cast<ICelestialObject>(NewBody);
	CelestialObjects.Add(Interface);
	
	AOrbitDebugActor::Get()->AddID(NewBody->GetID());
	if (currentPerspective == 0)
			AOrbitDebugActor::Get()->DrawOrbits();
	

	return NewBody;
}

ANiagaraActor* ACelestialGameMode::AddAsteroidSystem(FTransform Transform, UNiagaraSystem* System, bool bStartPaused)
{
	ANiagaraActor* NewSystem = GetWorld()->SpawnActor<ANiagaraActor>(ANiagaraActor::StaticClass(), Transform);

	if (UNiagaraComponent* NiagaraComponent = NewSystem->GetNiagaraComponent())
	{
		NiagaraComponent->SetAsset(System);
		NiagaraComponent->SetPaused(bStartPaused);
	}
	
	Asteroids.Add(NewSystem);
	
	return NewSystem;
}

void ACelestialGameMode::RemoveAsteroidSystem(ANiagaraActor* SystemToRemove)
{
	if (!SystemToRemove)
	{
		return;
	}
	
	int32 Removed = Asteroids.Remove(SystemToRemove);
	if (Removed == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to remove asteroid system that isn't tracked! (%s)"), *SystemToRemove->GetName());
	}
	
	SystemToRemove->Destroy();
}

ACelestialBody* ACelestialGameMode::DuplicateBody(ACelestialBody* BodyToDuplicate)
{
	AOrbitDebugActor::Get()->ManualStop = true;
	FTransform NewTransform = FTransform(
		BodyToDuplicate->GetActorRotation(), 
		BodyToDuplicate->GetActorLocation() + FVector(0, 0, (BodyToDuplicate->GetBodyRadius() * 2) + 25)
		);
	ACelestialBody* NewBody = AddBody(BodyToDuplicate->GetClass(), FName(FString(BodyToDuplicate->GetBodyName().ToString() + "_Duplicate")), NewTransform, false);

	// Duplicate physics parameters
	NewBody->SetMass(BodyToDuplicate->GetMass());
	NewBody->rotationRate = BodyToDuplicate->rotationRate;
	NewBody->SetCurrentVelocity(BodyToDuplicate->GetCurrentVelocity());

	// Make sure if the BodyToDuplicate is part of a body system that the new body is also added to that system
	for (FBodySystem BodySystem : BodySystems)
	{
		if (UBodySystemFunctionLibrary::DoesSystemContainBody(BodySystem, BodyToDuplicate))
		{
			UBodySystemFunctionLibrary::AddIDs(BodySystem, { NewBody });
			break;
		}
	}

	// Duplicate components
	for (UActorComponent* Component : BodyToDuplicate->GetComponents().Array())
	{
		if (Component->GetClass() == UAtmosphereComponent::StaticClass())
		{
			UAtmosphereComponent* NewAtmosphere = NewObject<UAtmosphereComponent>(NewBody);
			NewAtmosphere->SetProperties(Cast<UAtmosphereComponent>(Component)->GetProperties());
			NewBody->AddCelestialComponent(NewAtmosphere);
		}
		else if (Component->GetClass() == URingSystemComponent::StaticClass())
		{
			URingSystemComponent* NewRingSystem = NewObject<URingSystemComponent>(NewBody);
			URingSystemComponent* OldRingSystem = Cast<URingSystemComponent>(Component);
			NewRingSystem->SetRadius(OldRingSystem->GetRadius());
			NewRingSystem->SetWidth(OldRingSystem->GetWidth());
			NewRingSystem->SetGradient(OldRingSystem->GetGradient());
			NewBody->AddCelestialComponent(NewRingSystem);
			NewRingSystem->UpdateProperties();
		}
	}

	// Type specific duplication
	if (APlanet* NewPlanet = Cast<APlanet>(NewBody))
	{
		APlanet* OldPlanet = Cast<APlanet>(BodyToDuplicate);
		
		// First we need to unbind all of the delegates so they don't try to regen the planet mid-duplication
		NewPlanet->UnBindDelegates(); 

		// Duplicate BiomeColorSettings
		FBiomeColorSettings_ NewBiomeColorSettings;
		FBiomeColorSettings_ OldBiomeColorSettings = OldPlanet->ColorSettings->GetBiomeColorSettings()->GetStruct();
		NewBiomeColorSettings.bUsingNoise = OldBiomeColorSettings.bUsingNoise;
		NewBiomeColorSettings.NoiseOffset = OldBiomeColorSettings.NoiseOffset;
		NewBiomeColorSettings.NoiseStrength = OldBiomeColorSettings.NoiseStrength;
		NewBiomeColorSettings.blendAmount = OldBiomeColorSettings.blendAmount;
		UNoiseSettings* NewNoise = NewPlanet->CreateSettingsAsset<UNoiseSettings>(UNoiseSettings::StaticClass());
		NewNoise->SetStruct(
			FNoiseSettings_(
				OldBiomeColorSettings.Noise->GetStruct().FilterType,
				FSimpleNoiseSettings(
					OldBiomeColorSettings.Noise->GetStruct().SimpleNoiseSettings.Strength,
					OldBiomeColorSettings.Noise->GetStruct().SimpleNoiseSettings.BaseRoughness,
					OldBiomeColorSettings.Noise->GetStruct().SimpleNoiseSettings.Roughness,
					OldBiomeColorSettings.Noise->GetStruct().SimpleNoiseSettings.Center,
					OldBiomeColorSettings.Noise->GetStruct().SimpleNoiseSettings.numLayers,
					OldBiomeColorSettings.Noise->GetStruct().SimpleNoiseSettings.Persistence,
					OldBiomeColorSettings.Noise->GetStruct().SimpleNoiseSettings.MinValue
				),
				FRigidNoiseSettings(
					OldBiomeColorSettings.Noise->GetStruct().RigidNoiseSettings.Strength,
					OldBiomeColorSettings.Noise->GetStruct().RigidNoiseSettings.BaseRoughness,
					OldBiomeColorSettings.Noise->GetStruct().RigidNoiseSettings.Roughness,
					OldBiomeColorSettings.Noise->GetStruct().RigidNoiseSettings.Center,
					OldBiomeColorSettings.Noise->GetStruct().RigidNoiseSettings.numLayers,
					OldBiomeColorSettings.Noise->GetStruct().RigidNoiseSettings.Persistence,
					OldBiomeColorSettings.Noise->GetStruct().RigidNoiseSettings.MinValue,
					OldBiomeColorSettings.Noise->GetStruct().RigidNoiseSettings.WeightMultiplier
				)
			)
		);
		NewBiomeColorSettings.Noise = NewNoise;

		NewPlanet->ColorSettings->GetBiomeColorSettings()->SetStruct(NewBiomeColorSettings);
		// Duplicate all biomes
		NewPlanet->ColorSettings->GetBiomeColorSettings()->RemoveAllBiomes();
		for (UBiome* OldBiome : OldPlanet->ColorSettings->GetBiomeColorSettings()->GetBiomes())
		{
			UBiome* NewBiome = NewPlanet->CreateSettingsAsset<UBiome>(UBiome::StaticClass());
			FBiome_ NewStruct;
			FBiome_ OldStruct = OldBiome->GetStruct();
			memcpy(&NewStruct, &OldStruct, sizeof(FBiome_));
			NewBiome->SetStruct(NewStruct);
			NewPlanet->ColorSettings->GetBiomeColorSettings()->AddBiome(NewBiome);
		}
		// Duplicate color settings
		FColorSettings_ NewColorSettings;
		NewColorSettings.OceanColor = OldPlanet->ColorSettings->GetStruct().OceanColor;
		NewColorSettings.BiomeColorSettings = NewPlanet->ColorSettings->GetBiomeColorSettings();
		NewPlanet->ColorSettings->SetStruct(NewColorSettings);

		// Duplicate shape settings
		FShapeSettings_ NewShapeSettings;
		NewShapeSettings.PlanetRadius = OldPlanet->ShapeSettings->GetRadius();
		NewPlanet->ShapeSettings->SetStruct(NewShapeSettings);
		// Duplicate noise layers
		NewPlanet->ShapeSettings->RemoveAllNoiseLayers();
		for (UNoiseLayer* OldLayer : OldPlanet->ShapeSettings->GetNoiseLayers())
		{
			GetGameInstance<UCelestialGameInstance>()->CopyNoiseLayer(OldLayer);
			UNoiseLayer* NewLayer = NewPlanet->CreateSettingsAsset<UNoiseLayer>(UNoiseLayer::StaticClass());
			UNoiseSettings* NewSettings = NewPlanet->CreateSettingsAsset<UNoiseSettings>(UNoiseSettings::StaticClass());
			NewLayer->NoiseSettings = NewSettings;
			GetGameInstance<UCelestialGameInstance>()->PasteNoiseLayer(NewLayer);
			NewPlanet->ShapeSettings->AddNoiseLayer(NewLayer);
		}

		NewPlanet->BindDelegates();
		NewPlanet->ReGenerate();

		AOrbitDebugActor::Get()->ManualStop = false;
		if (currentPerspective == 0)
			AOrbitDebugActor::Get()->DrawOrbits();
		return NewPlanet;
	}
	else if (AGasGiant* NewGasGiant = Cast<AGasGiant>(NewBody))
	{
		FGasGiantColorSettings CopiedColorSettings;
		memcpy(&CopiedColorSettings, &Cast<AGasGiant>(BodyToDuplicate)->ColorSettings, sizeof(FGasGiantColorSettings));
		NewGasGiant->SetRadius(BodyToDuplicate->GetBodyRadius() / 100);
		NewGasGiant->ColorSettings = CopiedColorSettings;
		NewGasGiant->ReInit();
		if (currentPerspective == 0)
			NewGasGiant->DynamicMaterial->SetScalarParameterValue("bIsPaused", 1);

		AOrbitDebugActor::Get()->ManualStop = false;
		if (currentPerspective == 0)
			AOrbitDebugActor::Get()->DrawOrbits();
		return NewGasGiant;
	}
	else if (AStar* NewStar = Cast<AStar>(NewBody))
	{
		FStarProperties CopiedStarProperties;
#if PLATFORM_WINDOWS
		memcpy(&CopiedStarProperties, &Cast<AStar>(BodyToDuplicate)->starProperties, sizeof(FStarProperties));
#else // Linux and Mac don't like memcpy()
		CopiedStarProperties = FStarProperties(Cast<AStar>(BodyToDuplicate)->starProperties);
#endif
		NewStar->SetStarProperties(CopiedStarProperties);

		AOrbitDebugActor::Get()->ManualStop = false;
		if (currentPerspective == 0)
			AOrbitDebugActor::Get()->DrawOrbits();
		return NewStar;
	}

	return NewBody;
}

void ACelestialGameMode::RemoveBody(FString Body)
{
	ACelestialBody* BodyPtr = GetBodyByName(Body);
	if (BodyPtr)
	{
		AOrbitDebugActor::Get()->RemoveID(BodyPtr->GetID());
		
		Bodies.Remove(BodyPtr);
		CelestialObjects.Remove(Cast<ICelestialObject>(BodyPtr));

		if (currentPerspective == 0)
		{
			AOrbitDebugActor::Get()->DrawOrbits();
		}
		
		if (AStar* Star = Cast<AStar>(BodyPtr))
		{
			Stars.Remove(Star);

			// Reindex other stars
			for (int i = 0; i < Stars.Num(); ++i)
			{
				Stars[i]->SetStarNum(i);
			}
		}
		
		BodyPtr->Destroy();

		UE_LOG(LogTemp, Warning, TEXT("Removed %s"), *BodyPtr->GetBodyName().ToString());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Could not find body %s to remove"), *Body);
}

void ACelestialGameMode::SetGravitationalConstant(float NewG)
{
	gravitationalConstant = NewG;
	
	for (auto& System : Asteroids)
	{
		Cast<ANiagaraActor>(System)->GetNiagaraComponent()->SetVariableFloat(FName("GravitationalConstant"), gravitationalConstant);
	}
	if (currentPerspective == 0)
		AOrbitDebugActor::Get()->DrawOrbits();
}

bool ACelestialGameMode::SetAsteroidFieldActor(ANiagaraActor* NewAsteroidFieldActor)
{
	if (!AsteroidFieldActor)
	{
		return false;
	}
	if (Asteroids.Contains(NewAsteroidFieldActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to set asteroid field actor that is already tracked! (%s)"), *NewAsteroidFieldActor->GetName());
		return false;
	}
	
	AsteroidFieldActor = NewAsteroidFieldActor;
	Asteroids.Add(NewAsteroidFieldActor);
	
	bool bValidNiagaraVar = false;
	AsteroidFieldSpawnCount = AsteroidFieldActor->GetNiagaraComponent()->GetVariableInt(FName("SpawnCount"), bValidNiagaraVar);
	
	return true;
}

void ACelestialGameMode::SetPerspective(uint8 perspective)
{
	if (bGamePaused)
		return;

	ensure(PC);
	switch (perspective)
	{
	case 0: // Overview mode
	{
		PC->Possess(OverviewPlayer);
		CelestialWidget->RemoveFromParent();
		OverviewWidget->AddToViewport(0);
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(OverviewWidget->TakeWidget());
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		currentPerspective = perspective;
		AOrbitDebugActor::Get()->DrawOrbits();

		for (auto& System : Asteroids)
		{
			System->GetNiagaraComponent()->SetPaused(true);
		}

		for (AStar* Star : Stars)
		{
			Star->dynamicMaterial->SetScalarParameterValue("bIsPaused", 1);
			Star->GetParticleComp()->SetPaused(true);
		}

		TArray<AActor*> GasGiants;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGasGiant::StaticClass(), GasGiants);
		for (AActor* GasGiant : GasGiants)
		{
			(Cast<AGasGiant>(GasGiant))->DynamicMaterial->SetScalarParameterValue("bIsPaused", 1);
		}

		OnPerspectiveChanged.Broadcast(perspective);
		break;
	}
	case 1: // Celestial mode
	{
		PC->Possess(CelestialPlayer);
		OverviewWidget->RemoveFromParent();
		CelestialWidget->AddToViewport(0);
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		currentPerspective = perspective;
		AOrbitDebugActor::Get()->ClearOrbits();

		for (auto& System : Asteroids)
		{
			System->GetNiagaraComponent()->SetPaused(false);
		}

		for (AStar* Star : Stars)
		{
			Star->dynamicMaterial->SetScalarParameterValue("bIsPaused", 0);
			Star->GetParticleComp()->SetPaused(false);
		}

		TArray<AActor*> GasGiants;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGasGiant::StaticClass(), GasGiants);
		for (AActor* GasGiant : GasGiants)
		{
			(Cast<AGasGiant>(GasGiant))->DynamicMaterial->SetScalarParameterValue("bIsPaused", 0);
		}

		OnPerspectiveChanged.Broadcast(perspective);
		break;
	}
	default:
		break;
	}
}

ACelestialBody* ACelestialGameMode::GetBodyByName(FString Name)
{
	for (auto& body : Bodies)
	{
		if (body->GetBodyName().ToString() == Name)
			return body;
	}
	return nullptr;
}

ISettingsAssetID* ACelestialGameMode::GetAssetByID(uint32 ID)
{
	for (auto& body : Bodies)
	{
		if (body->GetID() == ID)
			return body;
	}
	TArray<FAssetData> AssetData;
	FAssetRegistryModule::GetRegistry().GetAssetsByPath("/Game/DataAssets", AssetData, true);
	TArray<ISettingsAssetID*> AssetIDs;
	for (FAssetData Asset : AssetData)
	{
		AssetIDs.Add(Cast<ISettingsAssetID>(Asset.GetAsset()));
	}
	for (ISettingsAssetID* Settings : AssetIDs)
	{
		if (Settings->GetID() == ID)
			return Settings;
	}
	return nullptr;
}

void ACelestialGameMode::LoadOnDiscAssetsOfClass(TArray<FDiscAssetRecord> LoadedAssets, TArray<FAssetData> OnDiscAssets, UClass* ClassToLoad)
{
	static TArray<FDiscAssetRecord> StaticLoadedAssets;
	static TArray<FAssetData> StaticOnDiscAssets;
	if (LoadedAssets.Num() > 0)
	{
		StaticLoadedAssets = LoadedAssets;
		StaticOnDiscAssets = OnDiscAssets;
	}

	for (FDiscAssetRecord Asset : StaticLoadedAssets)
	{
		if (Asset.Class == ClassToLoad)
		{
			for (FAssetData Data : StaticOnDiscAssets)
			{
				if (Data.GetSoftObjectPath() == Asset.ObjectPath)
				{
					FMemoryReader MemoryReader(Asset.AssetData);
					FCelestialSaveGameArchive Ar(MemoryReader);
					Data.GetAsset()->Serialize(Ar);
					break;
				}
			}
		}
	}
}

void ACelestialGameMode::LoadRuntimeAssetsOfClass(TArray<FAssetRecord> LoadedAssets, UClass* ClassToLoad)
{
	static TArray<FAssetRecord> StaticLoadedAssets;
	if (LoadedAssets.Num() > 0)
	{
		StaticLoadedAssets = LoadedAssets;
	}

	for (auto& Asset : StaticLoadedAssets)
	{
		if (Asset.Class == ClassToLoad)
		{
			UObject* NewSettings = APlanet::RestoreSettingsAsset(Asset.Name, Asset.AssetData, ClassToLoad);
			SettingsAssets.Add(Cast<USettingsAsset>(NewSettings));
		}
	}
	ApplySettingsAssets();
}

void ACelestialGameMode::ApplySettingsAssets()
{
	// This is so dumb
	for (USettingsAsset* Asset : SettingsAssets)
	{
		TArray<uint32> Ids = Asset->GetAppliedIDs();
		for (uint32 id : Ids)
		{
			if (UShapeSettings* ShapeSettings = Cast<UShapeSettings>(Asset))
			{
				if (Cast<APlanet>(GetAssetByID(id)))
				{
					Cast<APlanet>(GetAssetByID(id))->ShapeSettings = ShapeSettings;
				}
			}
			else if (UColorSettings* ColorSettings = Cast<UColorSettings>(Asset))
			{
				if (Cast<APlanet>(GetAssetByID(id)))
				{
					Cast<APlanet>(GetAssetByID(id))->ColorSettings = ColorSettings;
				}
			}
			else if (UBiomeColorSettings* BiomeColorSettings = Cast<UBiomeColorSettings>(Asset))
			{
				if (Cast<UColorSettings>(GetAssetByID(id)))
				{
					Cast<UColorSettings>(GetAssetByID(id))->SetBiomeColorSettings(BiomeColorSettings);
				}
			}
			else if (UBiome* Biome = Cast<UBiome>(Asset))
			{
				if (Cast<UBiomeColorSettings>(GetAssetByID(id)))
				{
					Cast<UBiomeColorSettings>(GetAssetByID(id))->AddBiome(Biome);
				}
			}
			else if (UNoiseLayer* NoiseLayer = Cast<UNoiseLayer>(Asset))
			{
				if (Cast<UShapeSettings>(GetAssetByID(id)))
				{
					Cast<UShapeSettings>(GetAssetByID(id))->AddNoiseLayer(NoiseLayer);
				}
			}
			else if (UNoiseSettings* NoiseSettings = Cast<UNoiseSettings>(Asset))
			{
				if (Cast<UNoiseLayer>(GetAssetByID(id)))
				{
					Cast<UNoiseLayer>(GetAssetByID(id))->NoiseSettings = NoiseSettings;
				}
				else if (Cast<UBiomeColorSettings>(GetAssetByID(id)))
				{
					Cast<UBiomeColorSettings>(GetAssetByID(id))->SetNoise(NoiseSettings);
				}
			}
		}
	}
	SettingsAssets.Empty();
}

void ACelestialGameMode::LoadGame()
{
	FAsyncLoadGameFromSlotDelegate OnLoadComplete;
	OnLoadComplete.BindLambda([this](const FString&, const int32, USaveGame* SaveGame) 
	{  
		if (UCelestialSaveGame* LoadedGame = Cast<UCelestialSaveGame>(SaveGame))
		{
			currentPerspective = 128;

			// The operation was successful, so LoadedGame now contains the data we saved earlier.
			UE_LOG(LogTemp, Warning, TEXT("LOADED"));

			// Load Linear Color Curves
			for (auto& Asset : LoadedGame->GradientAssets)
			{
				UCurveLinearColor* NewCurve = UColorCurveFunctionLibrary::CreateNewCurve(Asset.Name, Asset.AssetData);
			}

			// Restore Orbit Visualization Data
			AOrbitDebugActor* ODA = AOrbitDebugActor::Get();
			FMemoryReader ODAMemoryReader(LoadedGame->OrbitVisualizationData.ActorData);
			FCelestialSaveGameArchive ODAAr(ODAMemoryReader);
			ODA->Serialize(ODAAr);

			gravitationalConstant = LoadedGame->GravConst;
			SetAsteroidFieldNum(LoadedGame->AsteroidFieldNum);

			// Restore Celestial Body Data
			TArray<ACelestialBody*> RestoredBodies;
			for (auto& data : LoadedGame->CelestialBodyData)
			{
				bool BodyAlreadyExists = false;
				for (int32 i = 0; i < Bodies.Num(); i++)
				{
					if (Bodies[i]->GetBodyName().ToString() == data.Name.ToString())
					{
						FMemoryReader MemoryReader(data.ActorData);
						FCelestialSaveGameArchive Ar(MemoryReader);
						Bodies[i]->Serialize(Ar);
						Bodies[i]->SetActorTransform(data.Transform);
						RestoredBodies.Add(Bodies[i]);

						if (AGasGiant* GasGiant = Cast<AGasGiant>(Bodies[i]))
							GasGiant->ReInit();

						BodyAlreadyExists = true;
						break;
					}
				}
				if (!BodyAlreadyExists)
				{
					ACelestialBody* NewBody = AddBody(data.Class, NAME_None, data.Transform);

					FMemoryReader MemoryReader(data.ActorData);
					FCelestialSaveGameArchive Ar(MemoryReader);
					NewBody->Serialize(Ar);

					RestoredBodies.Add(NewBody);
					if (APlanet* planet = Cast<APlanet>(NewBody))
						planet->ClearSettingsAssets();
					else if (AGasGiant* GasGiant = Cast<AGasGiant>(NewBody))
						GasGiant->ReInit();
				}
				UE_LOG(LogTemp, Warning, TEXT("Data Loaded For: %s"), *data.Name.ToString());
			}
			TArray<ACelestialBody*> BodiesToDelete;
			for (ACelestialBody* Body : Bodies)
			{
				bool bBodyWasDeleted = true;
				for (ACelestialBody* RestoredBody : RestoredBodies)
				{
					if (RestoredBody->GetID() == Body->GetID() && RestoredBody->GetBodyName() != Body->GetBodyName())
						break;
					if (RestoredBody->GetID() == Body->GetID())
					{
						bBodyWasDeleted = false;
						break;
					}
				}
				if (bBodyWasDeleted)
					BodiesToDelete.Add(Body);
			}
			for (ACelestialBody* Body : BodiesToDelete)
			{
				RemoveBody(Body->GetBodyName().ToString());
			}

			
			// Load On Disc Settings Assets
			{
				TArray<FAssetData> AssetData;
				FAssetRegistryModule::GetRegistry().GetAssetsByPath("/Game/DataAssets", AssetData, true, true);
				LoadOnDiscAssetsOfClass(LoadedGame->OnDiscSettingsAssets, AssetData, UColorSettings::StaticClass());
				LoadOnDiscAssetsOfClass({}, {}, UBiomeColorSettings::StaticClass());
				LoadOnDiscAssetsOfClass({}, {}, UShapeSettings::StaticClass());
				LoadOnDiscAssetsOfClass({}, {}, UNoiseLayer::StaticClass());
				LoadOnDiscAssetsOfClass({}, {}, UNoiseSettings::StaticClass());
				LoadOnDiscAssetsOfClass({}, {}, UBiome::StaticClass());
			}
			// Load Runtime Settings Assets
			{
				LoadRuntimeAssetsOfClass(LoadedGame->SettingsAssets, UColorSettings::StaticClass());
				LoadRuntimeAssetsOfClass({}, UBiomeColorSettings::StaticClass());
				LoadRuntimeAssetsOfClass({}, UShapeSettings::StaticClass());
				LoadRuntimeAssetsOfClass({}, UNoiseLayer::StaticClass());
				LoadRuntimeAssetsOfClass({}, UNoiseSettings::StaticClass());
				LoadRuntimeAssetsOfClass({}, UBiome::StaticClass());
			}
			
			for (ACelestialBody* Body : Bodies) // Make sure the newly created and assigned asset's delegates are bound
			{
				if (APlanet* planet = Cast<APlanet>(Body))
					planet->BindDelegates();
			}

			// Restore BodySystems
			for (FBodySystemRecord System : LoadedGame->BodySystemsData)
			{
				FBodySystem NewSystem = FBodySystem(System.Name, System.SystemIDs);
				BodySystems.Add(NewSystem);
			}

			// Restore Celestial Player Data
			CelestialPlayer->SetActorTransform(LoadedGame->CelestialPlayerData.Transform);
			FMemoryReader CelMemoryReader(LoadedGame->CelestialPlayerData.ActorData);
			FCelestialSaveGameArchive CelAr(CelMemoryReader);
			CelestialPlayer->Serialize(CelAr);

			// Restore Overview Player Data
			OverviewPlayer->SetActorTransform(LoadedGame->OverviewPlayerData.Transform);
			FMemoryReader OrvwMemoryReader(LoadedGame->OverviewPlayerData.ActorData);
			FCelestialSaveGameArchive OrvwAr(OrvwMemoryReader);
			OverviewPlayer->Serialize(OrvwAr);
			OverviewPlayer->GetSpringArm()->TargetArmLength = LoadedGame->OverviewArmLength;
			OverviewPlayer->GetSpringArm()->SetRelativeRotation(LoadedGame->OverviewCameraRotation);

			// Restore components
			for (auto& CompData : LoadedGame->CelestialComponentData)
			{
				UE_LOG(LogTemp, Warning, TEXT("Started loading %s"), *CompData.Name.ToString());
				ACelestialBody* Parent = GetBodyByName(CompData.ParentName.ToString());

				UActorComponent* NewComponent = NewObject<UActorComponent>(Parent, CompData.Class, CompData.Name);
				FMemoryReader MemoryReader(CompData.ActorData);
				FCelestialSaveGameArchive Ar(MemoryReader);
				NewComponent->Serialize(Ar);
#if WITH_EDITOR
				NewComponent->CreationMethod = EComponentCreationMethod::Instance;
#endif
				Parent->AddCelestialComponent(Cast<UStaticMeshComponent>(NewComponent));

				// This could be an interface call?
				if (Cast<UAtmosphereComponent>(NewComponent))
				{
					Cast<UAtmosphereComponent>(NewComponent)->UpdateProperties();
				}
				else if (Cast<URingSystemComponent>(NewComponent))
				{
					Cast<URingSystemComponent>(NewComponent)->UpdateProperties();
				}
				UE_LOG(LogTemp, Warning, TEXT("Finished loading %s"), *CompData.Name.ToString());
			}

			currentPerspective = 1;

			TArray<FName> TerrestrialBodyNames;
			for (TActorIterator<ACelestialBody> Itr(GetWorld()); Itr; ++Itr) {
				if (APlanet* Planet = Cast<APlanet>(*Itr))
					TerrestrialBodyNames.Add(Planet->GetBodyName());
			}
			TerrestrialBodyNames.Sort([](const FName& a, const FName& b) { return b.FastLess(a); });
			GeneratePlanetsOrdered::bCurrentlyGenerating = false;
			GeneratePlanetsOrdered::DoGeneratePlanetsOrdered(TerrestrialBodyNames, this);
			}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NOT LOADED"));

			TArray<FName> TerrestrialBodyNames;
			for (TActorIterator<ACelestialBody> Itr(GetWorld()); Itr; ++Itr) 
			{
				if (APlanet* Planet = Cast<APlanet>(*Itr))
				{
					TerrestrialBodyNames.Add(Planet->GetBodyName());
				}
			}
			
			if (TerrestrialBodyNames.Num() > 0)
			{
				TerrestrialBodyNames.Sort([](const FName& a, const FName& b) { return b.FastLess(a); });
				GeneratePlanetsOrdered::bCurrentlyGenerating = false;
				GeneratePlanetsOrdered::DoGeneratePlanetsOrdered(TerrestrialBodyNames, this);
			}
			else
			{
				GetGameInstance<UCelestialGameInstance>()->StopLoadingScreen();
				SetPerspective(1);
				OnLoadingComplete.Broadcast();
			}
		}
	});
	FString SlotName = FString("Save" + FString::FromInt(Cast<UCelestialGameInstance>(GetGameInstance())->GetGameSlot()));
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, OnLoadComplete);
}

// ======= Runtime Console Commands ======================================================

void ACelestialGameMode::DeleteSave(int32 slot)
{
	FString SlotName = FString("Save" + FString::FromInt(slot));
	UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

void ACelestialGameMode::SaveAndQuit()
{
	FAsyncSaveGameToSlotDelegate OnSaveComplete;
	OnSaveComplete.BindLambda([](const FString&, const int32, bool succeeded) {  if (succeeded) FGenericPlatformMisc::RequestExit(false); });
	SaveAsync(OnSaveComplete);
}

void ACelestialGameMode::SaveAndQuitToMenu()
{
	FAsyncSaveGameToSlotDelegate OnSaveComplete;
	OnSaveComplete.BindLambda([this](const FString&, const int32, bool succeeded) { if (succeeded) UGameplayStatics::OpenLevel(GetWorld(), "MainMenu"); });
	SaveAsync(OnSaveComplete);
}

void ACelestialGameMode::SaveAsync(FAsyncSaveGameToSlotDelegate Out)
{
	if (UCelestialSaveGame* SaveGameInstance = Cast<UCelestialSaveGame>(UGameplayStatics::CreateSaveGameObject(UCelestialSaveGame::StaticClass())))
	{
		// Set data on the savegame object.

		SaveGameInstance->GravConst = gravitationalConstant;
		SaveGameInstance->AsteroidFieldNum = AsteroidFieldSpawnCount;

		// Save BodySystems
		SaveGameInstance->BodySystemsData.SetNum(BodySystems.Num());
		for (int32 i = 0; i < BodySystems.Num(); i++)
		{
			SaveGameInstance->BodySystemsData[i].Name = BodySystems[i].SystemName;
			SaveGameInstance->BodySystemsData[i].SystemIDs = BodySystems[i].BodyIDs;
		}

		// Save Celestial Body Data
		SaveGameInstance->CelestialBodyData.SetNum(Bodies.Num());
		for (int32 i = 0; i < Bodies.Num(); i++)
		{
			ACelestialBody* Body = Bodies[i];

			SaveGameInstance->CelestialBodyData[i].Class = Body->GetClass();
			SaveGameInstance->CelestialBodyData[i].Transform = Body->GetTransform();
			SaveGameInstance->CelestialBodyData[i].Name = (FName)Body->GetBodyName();

			FMemoryWriter MemoryWriter(SaveGameInstance->CelestialBodyData[i].ActorData);

			// Use a wrapper archive that converts FNames and UObject*'s to strings that can be read back in
			FCelestialSaveGameArchive Ar(MemoryWriter);

			// Serialize the object
			Body->Serialize(Ar);

			// Save celestial components
			for (int32 j = 0; j < Body->GetComponents().Num(); j++)
			{
				UActorComponent* Comp = Body->GetComponents().Array()[j];
				if (Cast<UAtmosphereComponent>(Comp) || Cast<URingSystemComponent>(Comp))
				{
					SaveGameInstance->CelestialComponentData.Add(FComponentRecord());

					SaveGameInstance->CelestialComponentData.Last().ParentName = Body->GetBodyName();
					SaveGameInstance->CelestialComponentData.Last().Class = Comp->GetClass();
					SaveGameInstance->CelestialComponentData.Last().Transform = FTransform();
					SaveGameInstance->CelestialComponentData.Last().Name = Comp->GetFName();

					FMemoryWriter CompMemoryWriter(SaveGameInstance->CelestialComponentData.Last().ActorData);
					FCelestialSaveGameArchive CompAr(CompMemoryWriter);
					Comp->Serialize(CompAr);
				}
			}
		}

		// Save Celestial Player Data
		SaveGameInstance->CelestialPlayerData.Class = CelestialPlayer->GetClass();
		SaveGameInstance->CelestialPlayerData.Transform = CelestialPlayer->GetTransform();
		SaveGameInstance->CelestialPlayerData.Name = FName("CelestialPlayer");
		FMemoryWriter CelMemoryWriter(SaveGameInstance->CelestialPlayerData.ActorData);
		FCelestialSaveGameArchive CelAr(CelMemoryWriter);
		CelestialPlayer->Serialize(CelAr);

		// Save Overview Player Data
		SaveGameInstance->OverviewPlayerData.Class = OverviewPlayer->GetClass();
		SaveGameInstance->OverviewPlayerData.Transform = OverviewPlayer->GetTransform();
		SaveGameInstance->OverviewPlayerData.Name = FName("OverviewPlayer");
		FMemoryWriter OrvwMemoryWriter(SaveGameInstance->OverviewPlayerData.ActorData);
		FCelestialSaveGameArchive OrvwAr(OrvwMemoryWriter);
		OverviewPlayer->Serialize(OrvwAr);
		SaveGameInstance->OverviewArmLength = OverviewPlayer->GetSpringArm()->TargetArmLength;
		SaveGameInstance->OverviewCameraRotation = OverviewPlayer->GetSpringArm()->GetRelativeRotation();

		// Save Orbit Visualization Data
		AOrbitDebugActor* ODA = AOrbitDebugActor::Get();
		SaveGameInstance->OrbitVisualizationData.Class = AOrbitDebugActor::Get()->GetClass();
		FMemoryWriter ODAMemoryWriter(SaveGameInstance->OrbitVisualizationData.ActorData);
		FCelestialSaveGameArchive ODAAr(ODAMemoryWriter);
		ODA->Serialize(ODAAr);

		// Save Gradients
		TArray<FAssetData> GradientsData;
		FAssetRegistryModule::GetRegistry().GetAssetsByPath("/Game/Materials/Gradients/Runtime", GradientsData, true, false);
		SaveGameInstance->GradientAssets.SetNum(GradientsData.Num());
		for (int32 i = 0; i < SaveGameInstance->GradientAssets.Num(); i++)
		{
			SaveGameInstance->GradientAssets[i].Class = GradientsData[i].GetClass();
			SaveGameInstance->GradientAssets[i].Name = GradientsData[i].AssetName;

			FMemoryWriter MemoryWriter(SaveGameInstance->GradientAssets[i].AssetData);
			FCelestialSaveGameArchive Ar(MemoryWriter);
			GradientsData[i].GetAsset()->Serialize(Ar);
		}

		// Save Settings Assets
		//NOTE This will save ALL assets even if they are not used (a.k.a. this is inefficient)
		TArray<FAssetData> AssetsData;
		FAssetRegistryModule::GetRegistry().GetAssetsByPath("/Game/DataAssets/Runtime", AssetsData, true);
		Algo::Sort(AssetsData, [](const FAssetData a, const FAssetData b) { return a.AssetName.FastLess(b.AssetName); });
		SaveGameInstance->SettingsAssets.SetNum(AssetsData.Num());
		for (int32 i = 0; i < SaveGameInstance->SettingsAssets.Num(); i++)
		{
			SaveGameInstance->SettingsAssets[i].Class = AssetsData[i].GetClass();
			SaveGameInstance->SettingsAssets[i].Name = AssetsData[i].AssetName; //NOTE Using 'AssetName' instead of local name so that order remains

			FMemoryWriter MemoryWriter(SaveGameInstance->SettingsAssets[i].AssetData);
			FCelestialSaveGameArchive Ar(MemoryWriter);
			AssetsData[i].GetAsset()->Serialize(Ar);
		}

		// Save On Disc Settings Assets
		TArray<FAssetData> OnDiscAssetsData;
		FAssetRegistryModule::GetRegistry().GetAssetsByPath("/Game/DataAssets", OnDiscAssetsData, true, true);
		SaveGameInstance->OnDiscSettingsAssets.SetNum(OnDiscAssetsData.Num());
		for (int32 i = 0; i < SaveGameInstance->OnDiscSettingsAssets.Num(); i++)
		{
			SaveGameInstance->OnDiscSettingsAssets[i].ObjectPath = OnDiscAssetsData[i].GetSoftObjectPath();
			SaveGameInstance->OnDiscSettingsAssets[i].Class = OnDiscAssetsData[i].GetClass();
			SaveGameInstance->OnDiscSettingsAssets[i].Name = OnDiscAssetsData[i].AssetName;

			FMemoryWriter MemoryWriter(SaveGameInstance->OnDiscSettingsAssets[i].AssetData);
			FCelestialSaveGameArchive Ar(MemoryWriter);
			OnDiscAssetsData[i].GetAsset()->Serialize(Ar);
		}

		// Save the data asynchronously
		FString SlotName = FString("Save" + FString::FromInt(Cast<UCelestialGameInstance>(GetGameInstance())->GetGameSlot()));
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SlotName, 0, Out);
	}
}

void ACelestialGameMode::OrbitDebug()
{
	if (currentPerspective != 0)
		SetPerspective(0);

	CreateWidget<UUserWidget, APlayerController*>(GetWorld()->GetFirstPlayerController(), OrbitDebugWidgetClass)->AddToViewport(0);
}

void ACelestialGameMode::ReGenAll()
{
	for (TActorIterator<APlanet> Itr(GetWorld()); Itr; ++Itr) {
		Cast<APlanet>(*Itr)->GeneratePlanet();
		Cast<APlanet>(*Itr)->ResetPosition();
	}
}

void ACelestialGameMode::ReGen(FString Planet)
{
	APlanet* Planet_ = Cast<APlanet>(GetBodyByName(Planet));
	if (Planet_)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReGen on: %s"), *Planet);
		Planet_->GeneratePlanet();
		Planet_->ResetPosition();
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("No body of name %s was found"), *Planet);
}

void ACelestialGameMode::ReBind(FString Planet)
{
	APlanet* Planet_ = Cast<APlanet>(GetBodyByName(Planet));
	if (Planet_)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReBind on: %s"), *Planet);
		Planet_->BindDelegates();
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("No body of name %s was found"), *Planet);
}

void ACelestialGameMode::tp(FString toPlanet)
{
	AActor* planet = GetBodyByName(toPlanet);
	if (planet)
		GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(planet->GetRootComponent()->GetComponentLocation());
}

void ACelestialGameMode::PauseGame()
{
	if (bGamePaused)
	{
		// Resume Game
		PauseWidget->RemoveFromParent();
		if (currentPerspective == 0)
		{
			OverviewWidget->AddToViewport(); 
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(OverviewWidget->TakeWidget());
			InputMode.SetHideCursorDuringCapture(false);
			PC->SetInputMode(InputMode);
		}
		else
		{ 			
			CelestialWidget->AddToViewport();
			PC->SetShowMouseCursor(false);
			PC->SetInputMode(FInputModeGameOnly());
		}

		UGameplayStatics::SetGamePaused(GetWorld(), false);
		bGamePaused = false;
	}
	else if (!GetWorld()->IsPaused())
	{
		// PauseGame
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		if (currentPerspective == 0)
			OverviewWidget->RemoveFromParent();
		else
			CelestialWidget->RemoveFromParent();
			
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(PauseWidget->TakeWidget());
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		PauseWidget->AddToViewport(1);
		bGamePaused = true;
	}
}

void ACelestialGameMode::SetAsteroidFieldNum(int32 num)
{
	AsteroidFieldSpawnCount = num;
	
	UNiagaraComponent* NiagaraComponent = AsteroidFieldActor->GetNiagaraComponent();
	if (!NiagaraComponent)
	{
		return;
	}
	
	bool WasPaused = NiagaraComponent->IsPaused();
	NiagaraComponent->SetVariableInt(FName("SpawnCount"), num);
	NiagaraComponent->ReinitializeSystem();
	NiagaraComponent->SetPaused(WasPaused);
}


// ======= End Runtime Console Commands ==================================================


void GeneratePlanetsOrdered::DoGeneratePlanetsOrdered(TArray<FName> PlanetNames, ACelestialGameMode* GM)
{
	if (!bCurrentlyGenerating)
	{
		bCurrentlyGenerating = true;

		GeneratedPlanets.Empty();
		TerrestrialPlanets = PlanetNames;
		GameMode = GM;

		// Set the delegate for when the planet is done genereating
		for (FName PlanetName : TerrestrialPlanets)
		{
			Cast<APlanet>(GameMode->GetBodyByName(PlanetName.ToString()))->OnPlanetGenerated.BindLambda([&](FName Name) {GeneratePlanetsOrdered::NewGeneratedPlanet(Name); });
		}
		if (TerrestrialPlanets.IsValidIndex(0))
			GameMode->ReGen(TerrestrialPlanets[0].ToString());
	}
}

void GeneratePlanetsOrdered::NewGeneratedPlanet(FName PlanetName)
{
	GeneratedPlanets.AddUnique(PlanetName);
	Cast<APlanet>(GameMode->GetBodyByName(PlanetName.ToString()))->OnPlanetGenerated.Unbind();
	GeneratedPlanets.Sort([](const FName& a, const FName& b) { return b.FastLess(a); });
	if (GeneratedPlanets == TerrestrialPlanets)
	{
		UGameplayStatics::SetGamePaused(GameMode->GetWorld(), false);
		bCurrentlyGenerating = false;
		GameMode->GetGameInstance<UCelestialGameInstance>()->StopLoadingScreen();
		GameMode->SetPerspective(1);
		GameMode->OnLoadingComplete.Broadcast();
		return;
	}
	else if (GeneratedPlanets.Num() < TerrestrialPlanets.Num())
		GameMode->ReGen(TerrestrialPlanets[GeneratedPlanets.Num()].ToString());
}
