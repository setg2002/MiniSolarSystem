// Copyright Soren Gilbertson

//TODO Remove unneccesary includes
#include "CelestialGameMode.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "SaveDataBlueprintFunctionLibrary.h"
#include "ColorCurveFunctionLibrary.h"
#include "CelestialSaveGameArchive.h"
#include "Curves/CurveLinearColor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "RingSystemComponent.h"
#include "AtmosphereComponent.h"
#include "AssetRegistryModule.h"
#include "CelestialSaveGame.h"
#include "NiagaraComponent.h"
#include "CelestialObject.h"
#include "CelestialPlayer.h"
#include "OverviewPlayer.h"
#include "ShapeSettings.h"
#include "CelestialBody.h"
#include "NoiseSettings.h"
#include "NiagaraActor.h"
#include "EngineUtils.h"
#include "NoiseLayer.h"
#include "Planet.h"
#include "Star.h"


float ACelestialGameMode::gravitationalConstant;

ACelestialGameMode::ACelestialGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	gravitationalConstant = 100;
}

void ACelestialGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlanetIlluminationInst = GetWorld()->GetParameterCollectionInstance(LoadObject<UMaterialParameterCollection>(NULL, TEXT("MaterialParameterCollection'/Game/MaterialStuff/PlanetIllumination.PlanetIllumination'"), NULL, LOAD_None, NULL));
	NumStars = 0;

	b = true;

	PC = GetWorld()->GetFirstPlayerController();
	
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// Make widgets
	CelestialWidget = CreateWidget<UUserWidget, APlayerController>(GetWorld()->GetFirstPlayerController(), CelestialWidgetClass);
	OverviewWidget = CreateWidget<UUserWidget, APlayerController>(GetWorld()->GetFirstPlayerController(), OverviewWidgetClass);

	CelestialPlayer = Cast<ACelestialPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ACelestialPlayer::StaticClass()));
	OverviewPlayer = Cast<AOverviewPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOverviewPlayer::StaticClass()));

	CelestialPlayer->SetWidget(CelestialWidget);

	// Gets all ACelestialBodies and adds them to bodies
	for (TActorIterator<ACelestialBody> Itr(GetWorld()); Itr; ++Itr) {
		bodies.Add(*Itr);

		APlanet* Planet = Cast<APlanet>(*Itr);
		if (Planet)
		{
			TerrestrialPlanets.Add(Planet->Name);
			Planet->OnPlanetGenerated.BindUFunction(this, "NewGeneratedPlanet");
		}
		else if (Cast<AStar>(*Itr))
		{
			AStar* Star = Cast<AStar>(*Itr);
			Star->SetStarNum(NumStars);
			NumStars++;
		}
	}
	TerrestrialPlanets.Sort([](const FName& a, const FName& b) { return b.FastLess(a); });
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
	
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		return;
	}

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
		{
			LargestBody = Body;
		}
		else if (Body->GetMass() > LargestBody->GetMass())
		{
			LargestBody = Body;
		}
	}
	TArray<AActor*> NiagaraSystems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANiagaraActor::StaticClass(), NiagaraSystems);
	for (auto& System : NiagaraSystems)
	{
		Cast<ANiagaraActor>(System)->GetNiagaraComponent()->SetNiagaraVariableVec3("GravityPos", LargestBody->GetActorLocation());
	}
}

void ACelestialGameMode::NewGeneratedPlanet(FName PlanetName)
{
	if (b)
	{
		GeneratedPlanets.AddUnique(PlanetName);

		GeneratedPlanets.Sort([](const FName& a, const FName& b) { return b.FastLess(a); });
		if (GeneratedPlanets == TerrestrialPlanets)
		{
			b = false;
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			return;
		}
		else if (TerrestrialPlanets.Num() > TerrestrialPlanets.Find(PlanetName) + 1)
		{
			ReGen(TerrestrialPlanets[TerrestrialPlanets.Find(PlanetName) + 1].ToString());
		}
	}
}

ACelestialBody* ACelestialGameMode::AddBody(TSubclassOf<ACelestialBody> Class, FName Name, FTransform Transform)
{
	ACelestialBody* NewBody = GetWorld()->SpawnActor<ACelestialBody>(Class, Transform);
	NewBody->Name = Name;

	bodies.Add(NewBody);

	APlanet* Planet = Cast<APlanet>(NewBody);
	if (Planet)
	{
		TerrestrialPlanets.Add(Planet->Name);
	}
	TerrestrialPlanets.Sort([](const FName& a, const FName& b) { return b.FastLess(a); });

	if (Cast<AStar>(NewBody))
	{
		AStar* Star = Cast<AStar>(NewBody);
		Star->SetStarNum(NumStars);
		NumStars++;
		PlanetIlluminationInst->SetScalarParameterValue("NumStars", NumStars);
	}

	const auto &Interface = Cast<ICelestialObject>(NewBody);
	celestialObjects.Add(Interface);

	if (currentPerspective == 0)
	{
		AOrbitDebugActor::Get()->DrawOrbits();
	}

	return NewBody;
}

void ACelestialGameMode::RemoveBody(FString Body)
{
	ACelestialBody* Body_ = GetBodyByName(Body);
	if (Body_)
	{
		Body_->Destroy();
		bodies.Remove(Body_);
		celestialObjects.Remove(Cast<ICelestialObject>(Body_));
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

		UE_LOG(LogTemp, Warning, TEXT("Removed %s"), *Body_->Name.ToString());
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
	{
		AOrbitDebugActor::Get()->DrawOrbits();
	}
}

void ACelestialGameMode::SetPerspective(uint8 perspective)
{
	ensure(PC);
	switch (perspective)
	{
	case 0: // Overview mode
	{
		PC->Possess(OverviewPlayer);
		CelestialWidget->RemoveFromViewport();
		OverviewWidget->AddToViewport(0);
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		currentPerspective = perspective;
		AOrbitDebugActor::Get()->DrawOrbits();

		TArray<UUserWidget*> Widgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, HighlightWidgetClass, false);
		for (UUserWidget* Widget : Widgets)
		{
			Widget->SetVisibility(ESlateVisibility::Visible);
		}

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

		TArray<UUserWidget*> Widgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, HighlightWidgetClass, false);
		for (UUserWidget* Widget : Widgets)
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}

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
		if (body->Name.ToString() == Name)
		{
			return body;
		}
	}
	return nullptr;
}

void ACelestialGameMode::LoadGame()
{
	// Retrieve and cast the USaveGame object to UMySaveGame.
	if (UCelestialSaveGame* LoadedGame = Cast<UCelestialSaveGame>(UGameplayStatics::LoadGameFromSlot("Save", 0)))
	{
		// The operation was successful, so LoadedGame now contains the data we saved earlier.
		UE_LOG(LogTemp, Warning, TEXT("LOADED"));

		// Load Linear Color Curves
		for (auto& Asset : LoadedGame->GradientAssets)
		{
			if (Asset.Class == UCurveLinearColor::StaticClass())
			{
				UCurveLinearColor* NewCurve = UColorCurveFunctionLibrary::CreateNewCurve(Asset.Name, Asset.AssetData);
			}
		}

		// Load Settings Assets
		for (auto& Asset : LoadedGame->SettingsAssets)
		{
			if (Asset.Class == UNoiseLayer::StaticClass())
			{
				UObject* NewSettings = APlanet::RestoreSettingsAsset<UNoiseLayer>(Asset.Name, Asset.AssetData);
			}
		}

		gravitationalConstant = LoadedGame->GravConst;

		// Restore Celestial Body Data
		for (auto& data : LoadedGame->CelestialBodyData)
		{
			bool BodyAlreadyExists = false;
			for (TActorIterator<ACelestialBody> Itr(GetWorld()); Itr; ++Itr)
			{
				if (Itr->GetName() == data.Name.ToString())
				{
					FMemoryReader MemoryReader(data.ActorData);
					FCelestialSaveGameArchive Ar(MemoryReader);
					Itr->Serialize(Ar);
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
			}
			UE_LOG(LogTemp, Warning, TEXT("Data Loaded For: %s"), *data.Name.ToString());
		}
		for (auto& CompData : LoadedGame->CelestialComponentData)
		{
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

		// Restore Orbit Visualization Data
		AOrbitDebugActor* ODA = AOrbitDebugActor::Get();
		FMemoryReader ODAMemoryReader(LoadedGame->OrbitVisualizationData.ActorData);
		FCelestialSaveGameArchive ODAAr(ODAMemoryReader);
		ODA->Serialize(ODAAr);

		ReGen(TerrestrialPlanets[0].ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NOT LOADED"));

		ReGen(TerrestrialPlanets[0].ToString());
	}
}


// ======= Runtime Console Commands ======================================================

void ACelestialGameMode::DeleteSave()
{
	UGameplayStatics::DeleteGameInSlot("Save", 0);
}

void ACelestialGameMode::SaveAndQuit()
{
	if (Save())
	{
		FGenericPlatformMisc::RequestExit(false);
	}
}

void ACelestialGameMode::SaveAndQuitToMenu()
{
	if (Save())
	{
		UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	}
}

bool ACelestialGameMode::Save()
{
	if (UCelestialSaveGame* SaveGameInstance = Cast<UCelestialSaveGame>(UGameplayStatics::CreateSaveGameObject(UCelestialSaveGame::StaticClass())))
	{
		// Set data on the savegame object.

		SaveGameInstance->GravConst = gravitationalConstant;

		// Save Celestial Body Data
		SaveGameInstance->CelestialBodyData.SetNum(bodies.Num());
		for (int32 i = 0; i < bodies.Num(); i++)
		{
			ACelestialBody* Body = bodies[i];

			SaveGameInstance->CelestialBodyData[i].Class = Body->GetClass();
			SaveGameInstance->CelestialBodyData[i].Transform = Body->GetTransform();
			SaveGameInstance->CelestialBodyData[i].Name = (FName)Body->GetName();

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

					SaveGameInstance->CelestialComponentData.Last().ParentName = Body->Name;
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
		FAssetRegistryModule::GetRegistry().GetAssetsByPath("/Game/MaterialStuff/Gradients/Runtime", GradientsData, true, false);
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
		TArray<FAssetData> AssetsData;
		FAssetRegistryModule::GetRegistry().GetAssetsByPath("/Game/DataAssets/Runtime", AssetsData, true, false);
		SaveGameInstance->SettingsAssets.SetNum(AssetsData.Num());
		for (int32 i = 0; i < SaveGameInstance->SettingsAssets.Num(); i++)
		{
			SaveGameInstance->SettingsAssets[i].Class = AssetsData[i].GetClass();
			if (AssetsData[i].GetClass() == UNoiseLayer::StaticClass()) //FIX Temporary name get
				SaveGameInstance->SettingsAssets[i].Name = Cast<UNoiseLayer>(AssetsData[i].GetAsset())->Name;
			else
				SaveGameInstance->SettingsAssets[i].Name = AssetsData[i].AssetName;

			FMemoryWriter MemoryWriter(SaveGameInstance->SettingsAssets[i].AssetData);
			FCelestialSaveGameArchive Ar(MemoryWriter);
			AssetsData[i].GetAsset()->Serialize(Ar);
		}

		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, "Save", 0))
		{
			// Save succeeded.
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Save Succeeded")));
			}
			return true;
		}
		else
		{
			// Save failed.
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Save Failed")));
			}
			return false;
		}
	}
	return false;
}

void ACelestialGameMode::OrbitDebug()
{
	if (currentPerspective != 0)
	{
		SetPerspective(0);
	}

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

void ACelestialGameMode::tp(FString toPlanet)
{
	AActor* planet = GetBodyByName(toPlanet);
	if (planet)
	{
		GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(planet->GetRootComponent()->GetComponentLocation());
	}
}

void ACelestialGameMode::SetTerrainResolution(FString Planet, int32 resolution)
{
	Cast<APlanet>(GetBodyByName(Planet))->resolution = resolution;
	Cast<APlanet>(GetBodyByName(Planet))->GeneratePlanet();
}
