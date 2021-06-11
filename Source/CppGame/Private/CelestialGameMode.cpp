// Copyright Soren Gilbertson

//TODO Remove unneccesary includes
#include "CelestialGameMode.h"
#include "Serialization\ObjectAndNameAsStringProxyArchive.h"
#include "GameFramework/SpringArmComponent.h"
#include "SaveDataBlueprintFunctionLibrary.h"
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

struct FCelestialSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
	FCelestialSaveGameArchive(FArchive& InInnerArchive)
		: FObjectAndNameAsStringProxyArchive(InInnerArchive, false)
	{
		ArIsSaveGame = true;
	}
};


ACelestialGameMode::ACelestialGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACelestialGameMode::BeginPlay()
{
	Super::BeginPlay();
	
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
	}
	TerrestrialPlanets.Sort([](const FName& a, const FName& b) { return b.FastLess(a); });

	// Gets all actors that implement ICelestialObject and adds them to celestialObjects
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GWorld, UCelestialObject::StaticClass(), Actors);
	for (auto& actor : Actors)
	{
		const auto &Interface = Cast<ICelestialObject>(actor);
		celestialObjects.Add(Interface);
	}

	LoadGame();

	SetPerspective(1);
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
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
		currentPerspective = perspective;
		AOrbitDebugActor::Get()->DrawOrbits();

		TArray<AActor*> NiagaraSystems;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANiagaraActor::StaticClass(), NiagaraSystems);
		for (auto& System : NiagaraSystems)
		{
			Cast<ANiagaraActor>(System)->GetNiagaraComponent()->SetPaused(true);
		}
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
		if (body->GetName() == Name)
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
		// Restore Celestial Body Data
		for (auto& Body : bodies)
		{
			UE_LOG(LogTemp, Warning, TEXT("Loading Data For: %s"), *Body->Name.ToString());
			for (auto& data : LoadedGame->CelestialBodyData)
			{
				if (data.Name == Body->Name)
				{
					Body->SetActorTransform(data.Transform);
					FMemoryReader MemoryReader(data.ActorData);
					FCelestialSaveGameArchive Ar(MemoryReader);
					Body->Serialize(Ar);
					UE_LOG(LogTemp, Warning, TEXT("Data Loaded For: %s"), *Body->Name.ToString());
					break;
				}
			}
		}
		/*for (auto& data : LoadedGame->CelestialBodyData)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Name = data.Name;
			ACelestialBody* NewBody = GetWorld()->SpawnActor<ACelestialBody>(
				data.Class,
				data.Transform,
				SpawnParameters);

			FMemoryReader MemoryReader(data.ActorData);
			FCelestialSaveGameArchive Ar(MemoryReader);
			NewBody->Serialize(Ar);

			UE_LOG(LogTemp, Warning, TEXT("Data Loaded For: %s"), *Body->Name.ToString());
		}*/
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
	Save();
	FGenericPlatformMisc::RequestExit(false);
}

void ACelestialGameMode::SaveAndQuitToMenu()
{
	Save();
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void ACelestialGameMode::Save()
{
	if (UCelestialSaveGame* SaveGameInstance = Cast<UCelestialSaveGame>(UGameplayStatics::CreateSaveGameObject(UCelestialSaveGame::StaticClass())))
	{
		// Set data on the savegame object.

		// Save Celestial Body Data
		SaveGameInstance->CelestialBodyData.SetNum(bodies.Num());
		for (int32 i = 0; i < bodies.Num(); i++)
		{
			ACelestialBody* Body = bodies[i];

			SaveGameInstance->CelestialBodyData[i].Class = Body->GetClass();
			SaveGameInstance->CelestialBodyData[i].Transform = Body->GetTransform();
			SaveGameInstance->CelestialBodyData[i].Name = Body->Name;

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

		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, "Save", 0))
		{
			// Save succeeded.
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Save Succeeded")));
			}
		}
		else
		{
			// Save failed.
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Save Failed")));
			}
		}
	}
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
	UE_LOG(LogTemp, Warning, TEXT("ReGen on: %s"), *Planet);
	Cast<APlanet>(GetBodyByName(Planet))->GeneratePlanet();
	Cast<APlanet>(GetBodyByName(Planet))->ResetPosition();
}

void ACelestialGameMode::tp(FString toPlanet)
{
	AActor* planet = GetBodyByName(toPlanet);
	if (planet)
	{
		GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(planet->GetActorLocation());
	}
}

void ACelestialGameMode::SetTerrainResolution(FString Planet, int32 resolution)
{
	Cast<APlanet>(GetBodyByName(Planet))->resolution = resolution;
	Cast<APlanet>(GetBodyByName(Planet))->GeneratePlanet();
}
