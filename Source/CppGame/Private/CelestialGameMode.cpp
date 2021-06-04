// Copyright Soren Gilbertson


#include "CelestialGameMode.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "AssetRegistryModule.h"
#include "CelestialSaveGame.h"
#include "NiagaraComponent.h"
#include "CelestialObject.h"
#include "CelestialPlayer.h"
#include "OverviewPlayer.h"
#include "CelestialBody.h"
#include "NiagaraActor.h"
#include "EngineUtils.h"
#include "Planet.h"


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
		if (!GeneratedPlanets.Contains(PlanetName))
		{
			GeneratedPlanets.Add(PlanetName);

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
	if (UCelestialSaveGame* LoadedGame = Cast<UCelestialSaveGame>(UGameplayStatics::LoadGameFromSlot("Test", 0)))
	{
		// The operation was successful, so LoadedGame now contains the data we saved earlier.
		UE_LOG(LogTemp, Warning, TEXT("LOADED"));

		for (auto& Object : celestialObjects)
		{
			if (Cast<ACelestialBody>(Object))
			{
				ACelestialBody* Body = Cast<ACelestialBody>(Object);
				Body->SetActorLocation(*LoadedGame->CelestialLocations.Find(Body));
				Body->SetCurrentVelocity(*LoadedGame->CelestialVelocities.Find(Body));
			}
		}
		CelestialPlayer->SetActorLocation(LoadedGame->PlayerLocation);
		CelestialPlayer->SetCurrentVelocity(LoadedGame->PlayerVelocity);
		CelestialPlayer->SetActorRotation(LoadedGame->PlayerRotation);
		CelestialPlayer->SetIgnoreGravity(LoadedGame->IgnoreGravity);
		CelestialPlayer->SetThrottle(LoadedGame->Throttle);

		OverviewPlayer->Speed = LoadedGame->Speed;
		OverviewPlayer->SetActorRotation(LoadedGame->OverviewRotation);
		OverviewPlayer->SetActorLocation(LoadedGame->OverviewLocation);
		OverviewPlayer->GetSpringArm()->TargetArmLength = LoadedGame->TargetArmLength;
		OverviewPlayer->GetSpringArm()->SetRelativeRotation(LoadedGame->SpringArmRotation);

		AOrbitDebugActor* ODA = AOrbitDebugActor::Get();
		ODA->SetDrawType(LoadedGame->DrawType);
		ODA->SetNumSteps(LoadedGame->NumSteps);
		ODA->SetTimeStep(LoadedGame->TimeStep);
		ODA->SetRelativeToBody(LoadedGame->bRelativeToBody);
		ODA->SetRelativeBody(LoadedGame->CentralBody);
		ODA->SetWidth(LoadedGame->Width);
		ODA->SetRenderedSteps(LoadedGame->RenderedSteps);
	}
	//else
	//{
		ReGen(TerrestrialPlanets[0].ToString());
	//}
}


// ======= Runtime Console Commands ======================================================

void ACelestialGameMode::DeleteSave()
{
	UGameplayStatics::DeleteGameInSlot("Test", 0);
}

void ACelestialGameMode::SaveAndQuit()
{
	Save();
	FGenericPlatformMisc::RequestExit(false);
}

void ACelestialGameMode::Save()
{
	if (UCelestialSaveGame* SaveGameInstance = Cast<UCelestialSaveGame>(UGameplayStatics::CreateSaveGameObject(UCelestialSaveGame::StaticClass())))
	{
		// Set data on the savegame object.
		for (auto& Object : celestialObjects)
		{
			if (Cast<ACelestialBody>(Object))
			{
				ACelestialBody* Body = Cast<ACelestialBody>(Object);
				SaveGameInstance->CelestialLocations.Add(Body, Body->GetActorLocation());
				SaveGameInstance->CelestialVelocities.Add(Body, Body->GetCurrentVelocity());
			}
		}
		SaveGameInstance->PlayerLocation = CelestialPlayer->GetActorLocation();
		SaveGameInstance->PlayerVelocity = CelestialPlayer->GetCurrentVelocity();
		SaveGameInstance->PlayerRotation = CelestialPlayer->GetActorRotation();
		SaveGameInstance->IgnoreGravity = CelestialPlayer->GetIgnoreGravity();
		SaveGameInstance->Throttle = CelestialPlayer->GetThrottle();

		SaveGameInstance->Speed = OverviewPlayer->Speed;
		SaveGameInstance->OverviewRotation = OverviewPlayer->GetActorRotation();
		SaveGameInstance->OverviewLocation = OverviewPlayer->GetActorLocation();
		SaveGameInstance->TargetArmLength = OverviewPlayer->GetSpringArm()->TargetArmLength;
		SaveGameInstance->SpringArmRotation = OverviewPlayer->GetSpringArm()->GetRelativeRotation();

		AOrbitDebugActor* ODA = AOrbitDebugActor::Get();
		SaveGameInstance->DrawType = ODA->GetDrawType();
		SaveGameInstance->NumSteps = ODA->GetNumSteps();
		SaveGameInstance->TimeStep = ODA->GetTimeStep();
		SaveGameInstance->bRelativeToBody = ODA->GetRelativeToBody();
		SaveGameInstance->CentralBody = ODA->GetRelativeBody();
		SaveGameInstance->Width = ODA->GetWidth();
		SaveGameInstance->RenderedSteps = ODA->GetRenderedSteps();


		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, "Test", 0))
		{
			// Save succeeded.
			if (GEngine)
			{
				FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
				TArray<FAssetData> AssetData;
				const UClass* Class = UDataAsset::StaticClass();
				AssetRegistryModule.Get().GetAssetsByClass(Class->GetFName(), AssetData);
				for (auto& Asset : AssetData)
				{
					FStringOutputDevice outputDevice;
					UPackage::SavePackage(
						Asset.GetPackage(), 
						nullptr, 
						EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, 
						*Asset.GetPackage()->FileName.ToString(), 
						&outputDevice
					);
				}

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Save Succedded")));
			}
		}
		else
		{
			// Save failed.
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Save Failed")));
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
	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(GetBodyByName(toPlanet)->GetActorLocation());
}

void ACelestialGameMode::SetTerrainResolution(FString Planet, int32 resolution)
{
	Cast<APlanet>(GetBodyByName(Planet))->resolution = resolution;
	Cast<APlanet>(GetBodyByName(Planet))->GeneratePlanet();
}
