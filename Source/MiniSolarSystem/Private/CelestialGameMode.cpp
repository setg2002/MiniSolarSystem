// Copyright Soren Gilbertson

//TODO Remove unneccesary includes
#include "CelestialGameMode.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "BodySystemFunctionLibrary.h"
#include "ColorCurveFunctionLibrary.h"
#include "BodySystemFunctionLibrary.h"
#include "CelestialSaveGameArchive.h"
#include "Curves/CurveLinearColor.h"
#include "GasGiantColorSettings.h"
#include "CelestialGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "RingSystemComponent.h"
#include "AtmosphereComponent.h"
#include "AssetRegistryModule.h"
#include "CelestialSaveGame.h"
#include "NiagaraComponent.h"
#include "OrbitDebugActor.h"
#include "CelestialObject.h"
#include "CelestialPlayer.h"
#include "OverviewPlayer.h"
#include "ColorSettings.h"
#include "ShapeSettings.h"
#include "CelestialBody.h"
#include "NoiseSettings.h"
#include "NiagaraActor.h"
#include "EngineUtils.h"
#include "NoiseLayer.h"
#include "GasGiant.h"
#include "Planet.h"
#include "Star.h"


float ACelestialGameMode::gravitationalConstant;

ACelestialGameMode::ACelestialGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	currentPerspective = 255;
	gravitationalConstant = 100;
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
	NumStars = 0;

	PC = GetWorld()->GetFirstPlayerController();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// Make widgets
	CelestialWidget = CreateWidget<UUserWidget, APlayerController>(GetWorld()->GetFirstPlayerController(), CelestialWidgetClass);
	OverviewWidget  = CreateWidget<UUserWidget, APlayerController>(GetWorld()->GetFirstPlayerController(), OverviewWidgetClass);
	PauseWidget = CreateWidget<UUserWidget, APlayerController>(GetWorld()->GetFirstPlayerController(), PauseWidgetClass);

	CelestialPlayer = Cast<ACelestialPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ACelestialPlayer::StaticClass()));
	OverviewPlayer = Cast<AOverviewPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOverviewPlayer::StaticClass()));

	CelestialPlayer->SetWidget(CelestialWidget);

	// Gets all ACelestialBodies and adds them to bodies
	for (TActorIterator<ACelestialBody> Itr(GetWorld()); Itr; ++Itr) {
		bodies.Add(*Itr);

		if (Cast<AStar>(*Itr))
		{
			AStar* Star = Cast<AStar>(*Itr);
			Star->SetStarNum(NumStars);
			NumStars++;
		}
	}
	PlanetIlluminationInst->SetScalarParameterValue("NumStars", NumStars);

	// Gets all actors that implement ICelestialObject and adds them to celestialObjects
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GWorld, UCelestialObject::StaticClass(), Actors);
	for (auto& actor : Actors)
	{
		const auto &Interface = Cast<ICelestialObject>(actor);
		celestialObjects.Add(Interface);
	}

	SetPerspective(1);

	LoadGame();
}

void ACelestialGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentPerspective == 1)
	{
		for (int i = 0; i < celestialObjects.Num(); i++) {
			ICelestialObject* thisObject = celestialObjects[i];

			thisObject->UpdateVelocity(bodies, DeltaTime);
			thisObject->UpdatePosition(DeltaTime);
		}
	}

	// Set the gravity location of asteroid Niagara systems to the location of the most massive body
	ACelestialBody* LargestBody = nullptr;
	for (auto& Body : bodies)
	{
		if (LargestBody == nullptr)
			LargestBody = Body;
		else if (Body->GetMass() > LargestBody->GetMass())
			LargestBody = Body;
	}
	TArray<AActor*> NiagaraSystems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANiagaraActor::StaticClass(), NiagaraSystems);
	for (auto& System : NiagaraSystems)
	{
		Cast<ANiagaraActor>(System)->GetNiagaraComponent()->SetNiagaraVariableVec3("GravityPos", LargestBody->GetActorLocation());
	}
}

ACelestialBody* ACelestialGameMode::AddBody(TSubclassOf<ACelestialBody> Class, FName Name, FTransform Transform)
{
	ACelestialBody* NewBody = GetWorld()->SpawnActor<ACelestialBody>(Class, Transform);
	NewBody->SetName(Name);

	bodies.Add(NewBody);

	if (AStar* Star = Cast<AStar>(NewBody))
	{
		Star->SetStarNum(NumStars);
		NumStars++;
		PlanetIlluminationInst->SetScalarParameterValue("NumStars", NumStars);
	}

	const auto &Interface = Cast<ICelestialObject>(NewBody);
	celestialObjects.Add(Interface);
	
	AOrbitDebugActor::Get()->AddID(NewBody->GetID());
	if (currentPerspective == 0)
			AOrbitDebugActor::Get()->DrawOrbits();
	

	return NewBody;
}

ACelestialBody* ACelestialGameMode::DuplicateBody(ACelestialBody* BodyToDuplicate)
{
	AOrbitDebugActor::Get()->ManualStop = true;
	FTransform NewTransform = FTransform(
		BodyToDuplicate->GetActorRotation(), 
		BodyToDuplicate->GetActorLocation() + FVector(0, 0, (BodyToDuplicate->GetBodyRadius() * 2) + 25)
		);
	ACelestialBody* NewBody = AddBody(BodyToDuplicate->GetClass(), FName(FString(BodyToDuplicate->GetBodyName().ToString() + "_Duplicate")), NewTransform);
	
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
		UNoiseSettings* NewNoise = Cast<UNoiseSettings>(NewPlanet->CreateSettingsAsset(UNoiseSettings::StaticClass()));
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
				FRidgidNoiseSettings(
					OldBiomeColorSettings.Noise->GetStruct().RidgidNoiseSettings.Strength,
					OldBiomeColorSettings.Noise->GetStruct().RidgidNoiseSettings.BaseRoughness,
					OldBiomeColorSettings.Noise->GetStruct().RidgidNoiseSettings.Roughness,
					OldBiomeColorSettings.Noise->GetStruct().RidgidNoiseSettings.Center,
					OldBiomeColorSettings.Noise->GetStruct().RidgidNoiseSettings.numLayers,
					OldBiomeColorSettings.Noise->GetStruct().RidgidNoiseSettings.Persistence,
					OldBiomeColorSettings.Noise->GetStruct().RidgidNoiseSettings.MinValue,
					OldBiomeColorSettings.Noise->GetStruct().RidgidNoiseSettings.WeightMultiplier
				)
			)
		);
		NewBiomeColorSettings.Noise = NewNoise;

		NewPlanet->ColorSettings->GetBiomeColorSettings()->SetStruct(NewBiomeColorSettings);
		// Duplicate all biomes
		NewPlanet->ColorSettings->GetBiomeColorSettings()->RemoveAllBiomes();
		for (UBiome* OldBiome : OldPlanet->ColorSettings->GetBiomeColorSettings()->GetBiomes())
		{
			UBiome* NewBiome = Cast<UBiome>(NewPlanet->CreateSettingsAsset(UBiome::StaticClass()));
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
			UNoiseLayer* NewLayer = Cast<UNoiseLayer>(NewPlanet->CreateSettingsAsset(UNoiseLayer::StaticClass()));
			UNoiseSettings* NewSettings = Cast<UNoiseSettings>(NewPlanet->CreateSettingsAsset(UNoiseSettings::StaticClass()));
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

		AOrbitDebugActor::Get()->ManualStop = false;
		if (currentPerspective == 0)
			AOrbitDebugActor::Get()->DrawOrbits();
		return NewGasGiant;
	}
	else if (AStar* NewStar = Cast<AStar>(NewBody))
	{
		FStarProperties CopiedStarProperties;
		memcpy(&CopiedStarProperties, &Cast<AStar>(BodyToDuplicate)->starProperties, sizeof(FStarProperties));
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
	ACelestialBody* Body_ = GetBodyByName(Body);
	if (Body_)
	{
		AOrbitDebugActor::Get()->RemoveID(Body_->GetID());

		Body_->Destroy();
		bodies.Remove(Body_);
		celestialObjects.Remove(Cast<ICelestialObject>(Body_));

		if (currentPerspective == 0)
			AOrbitDebugActor::Get()->DrawOrbits();

		if (Cast<AStar>(Body_))
		{
			NumStars = 0;
			for (TActorIterator<AStar> Itr(GetWorld()); Itr; ++Itr) {
				AStar* Star = Cast<AStar>(*Itr);
				Star->SetStarNum(NumStars);
				NumStars++;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Removed %s"), *Body_->GetBodyName().ToString());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Could not find body %s to remove"), *Body);
}

void ACelestialGameMode::SetGravitationalConstant(float NewG)
{
	gravitationalConstant = NewG;
	TArray<AActor*> NiagaraSystems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANiagaraActor::StaticClass(), NiagaraSystems);
	for (auto& System : NiagaraSystems)
	{
		Cast<ANiagaraActor>(System)->GetNiagaraComponent()->SetNiagaraVariableFloat("GravitationalConstant", gravitationalConstant);
	}
	if (currentPerspective == 0)
		AOrbitDebugActor::Get()->DrawOrbits();
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
		CelestialWidget->RemoveFromViewport();
		OverviewWidget->AddToViewport(0);
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(OverviewWidget->TakeWidget());
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		currentPerspective = perspective;
		AOrbitDebugActor::Get()->DrawOrbits();

		TArray<AActor*> NiagaraSystems;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANiagaraActor::StaticClass(), NiagaraSystems);
		for (auto& System : NiagaraSystems)
		{
			Cast<ANiagaraActor>(System)->GetNiagaraComponent()->SetPaused(true);
		}

		TArray<AActor*> Stars;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStar::StaticClass(), Stars);
		for (AActor* Star : Stars)
		{
			Cast<AStar>(Star)->dynamicMaterial->SetScalarParameterValue("bIsPaused", 1);
			Cast<AStar>(Star)->GetParticleComp()->SetPaused(true);
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
		OverviewWidget->RemoveFromViewport();
		CelestialWidget->AddToViewport(0);
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		currentPerspective = perspective;
		AOrbitDebugActor::Get()->ClearOrbits();

		TArray<AActor*> NiagaraSystems;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANiagaraActor::StaticClass(), NiagaraSystems);
		for (auto& System : NiagaraSystems)
		{
			Cast<ANiagaraActor>(System)->GetNiagaraComponent()->SetPaused(false);
		}

		TArray<AActor*> Stars;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStar::StaticClass(), Stars);
		for (AActor* Star : Stars)
		{
			Cast<AStar>(Star)->dynamicMaterial->SetScalarParameterValue("bIsPaused", 0);
			Cast<AStar>(Star)->GetParticleComp()->SetPaused(false);
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
	for (auto& body : bodies)
	{
		if (body->GetBodyName().ToString() == Name)
			return body;
	}
	return nullptr;
}

ISettingsAssetID* ACelestialGameMode::GetAssetByID(uint32 ID)
{
	for (auto& body : bodies)
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
				if (Data.ObjectPath == Asset.ObjectPath)
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
			UObject* NewSettings = APlanet::RestoreSettingsAsset<UColorSettings>(Asset.Name, Asset.AssetData);
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
				for (int32 i = 0; i < bodies.Num(); i++)
				{
					if (bodies[i]->GetBodyName().ToString() == data.Name.ToString())
					{
						FMemoryReader MemoryReader(data.ActorData);
						FCelestialSaveGameArchive Ar(MemoryReader);
						bodies[i]->Serialize(Ar);
						bodies[i]->SetActorTransform(data.Transform);
						RestoredBodies.Add(bodies[i]);

						if (AGasGiant* GasGiant = Cast<AGasGiant>(bodies[i]))
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
			for (ACelestialBody* Body : bodies)
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
			
			for (ACelestialBody* Body : bodies) // Make sure the newly created and assigned asset's delegates are bound
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
			GeneratePlanetsOrdered::DoGeneratePlanetsOrdered(TerrestrialBodyNames, this);
			}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NOT LOADED"));

			TArray<FName> TerrestrialBodyNames;
			for (TActorIterator<ACelestialBody> Itr(GetWorld()); Itr; ++Itr) {
				if (APlanet* Planet = Cast<APlanet>(*Itr))
					TerrestrialBodyNames.Add(Planet->GetBodyName());
			}
			TerrestrialBodyNames.Sort([](const FName& a, const FName& b) { return b.FastLess(a); });
			GeneratePlanetsOrdered::DoGeneratePlanetsOrdered(TerrestrialBodyNames, this);
		}
	});
	UGameplayStatics::AsyncLoadGameFromSlot("Save", 0, OnLoadComplete);
}

// ======= Runtime Console Commands ======================================================

void ACelestialGameMode::DeleteSave()
{
	UGameplayStatics::DeleteGameInSlot("Save", 0);
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
		SaveGameInstance->AsteroidFieldNum = AsteroidFieldNum;

		// Save BodySystems
		SaveGameInstance->BodySystemsData.SetNum(BodySystems.Num());
		for (int32 i = 0; i < BodySystems.Num(); i++)
		{
			SaveGameInstance->BodySystemsData[i].Name = BodySystems[i].SystemName;
			SaveGameInstance->BodySystemsData[i].SystemIDs = BodySystems[i].BodyIDs;
		}

		// Save Celestial Body Data
		SaveGameInstance->CelestialBodyData.SetNum(bodies.Num());
		for (int32 i = 0; i < bodies.Num(); i++)
		{
			ACelestialBody* Body = bodies[i];

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
			SaveGameInstance->OnDiscSettingsAssets[i].ObjectPath = OnDiscAssetsData[i].ObjectPath;
			SaveGameInstance->OnDiscSettingsAssets[i].Class = OnDiscAssetsData[i].GetClass();
			SaveGameInstance->OnDiscSettingsAssets[i].Name = OnDiscAssetsData[i].AssetName;

			FMemoryWriter MemoryWriter(SaveGameInstance->OnDiscSettingsAssets[i].AssetData);
			FCelestialSaveGameArchive Ar(MemoryWriter);
			OnDiscAssetsData[i].GetAsset()->Serialize(Ar);
		}

		// Save the data asynchronously
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, "Save", 0, Out);
	}
}

void ACelestialGameMode::OrbitDebug()
{
	if (currentPerspective != 0)
		SetPerspective(0);

	CreateWidget<UUserWidget, APlayerController>(GetWorld()->GetFirstPlayerController(), OrbitDebugWidgetClass)->AddToViewport(0);
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

void ACelestialGameMode::SetTerrainResolution(FString PlanetName, int32 resolution)
{
	APlanet* Planet = Cast<APlanet>(GetBodyByName(PlanetName));
	if (Planet)
	{
		Planet->resolution = resolution;
		Planet->GeneratePlanet();
	}
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
			OverviewWidget->RemoveFromViewport();
		else
			CelestialWidget->RemoveFromViewport();
			
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
	AsteroidFieldNum = num;
	bool WasPaused = AsteroidFieldActor->GetNiagaraComponent()->IsPaused();
	AsteroidFieldActor->GetNiagaraComponent()->SetVariableInt(FName("SpawnCount"), num);
	AsteroidFieldActor->GetNiagaraComponent()->ReinitializeSystem();
	AsteroidFieldActor->GetNiagaraComponent()->SetPaused(WasPaused);
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
	GeneratedPlanets.Add(PlanetName);

	GeneratedPlanets.Sort([](const FName& a, const FName& b) { return b.FastLess(a); });
	if (GeneratedPlanets == TerrestrialPlanets)
	{
		for (FName Name : TerrestrialPlanets)
		{
			Cast<APlanet>(GameMode->GetBodyByName(Name.ToString()))->OnPlanetGenerated.Unbind();
		}
		UGameplayStatics::SetGamePaused(GameMode->GetWorld(), false);
		bCurrentlyGenerating = false;
		GameMode->GetGameInstance<UCelestialGameInstance>()->StopLoadingScreen();
		GameMode->OnLoadingComplete.Broadcast();
		return;
	}
	else if (GeneratedPlanets.Num() < TerrestrialPlanets.Num())
		GameMode->ReGen(TerrestrialPlanets[GeneratedPlanets.Num()].ToString());
}
