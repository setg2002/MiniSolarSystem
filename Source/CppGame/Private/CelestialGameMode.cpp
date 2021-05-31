// Copyright Soren Gilbertson


#include "CelestialGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
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

	SetPerspective(1);

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

	ReGen(TerrestrialPlanets[0].ToString());

	// Gets all actors that implement ICelestialObject and adds them to celestialObjects
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GWorld, UCelestialObject::StaticClass(), Actors);
	for (auto& actor : Actors)
	{
		const auto &Interface = Cast<ICelestialObject>(actor);
		celestialObjects.Add(Interface);
	}
}

void ACelestialGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*if (AOrbitDebugActor::Get()->DrawType != EDrawType::Ribbon && AOrbitDebugActor::Get()->bAutoDraw)
	{
		AOrbitDebugActor::Get()->DrawOrbits();
	}*/
	
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

}

/*TArray<FName>*/void ACelestialGameMode::NewGeneratedPlanet(FName PlanetName)
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


// ======= Runtime Console Commands ======================================================

void ACelestialGameMode::ReGenAll()
{
	for (TActorIterator<APlanet> Itr(GetWorld()); Itr; ++Itr) {
		Cast<APlanet>(*Itr)->GeneratePlanet();
		Cast<APlanet>(*Itr)->ResetPosition();
	}
}

void ACelestialGameMode::ReGen(FString Planet)
{
	for (TActorIterator<APlanet> Itr(GetWorld()); Itr; ++Itr) {
		if (Cast<AActor>(*Itr)->GetName() == Planet)
		{
			UE_LOG(LogTemp, Warning, TEXT("ReGen on: %s"), *Planet);
			Cast<APlanet>(*Itr)->GeneratePlanet();
			Cast<APlanet>(*Itr)->ResetPosition();
		}
	}
}

void ACelestialGameMode::tp(FString toPlanet)
{
	for (auto& planet : bodies)
	{
		if (planet->GetName() == toPlanet)
		{
			GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(planet->GetActorLocation());
		}
	}
}

void ACelestialGameMode::SetOrbitDebugMethod(EDrawType DrawType)
{
	AOrbitDebugActor::Get()->DrawType = DrawType;

	if (currentPerspective == 0)
	{
		AOrbitDebugActor::Get()->DrawOrbits();
	}
}
