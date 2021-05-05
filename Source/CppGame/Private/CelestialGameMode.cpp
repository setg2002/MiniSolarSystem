// This is a copyright notice


#include "CelestialGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "OrbitDebugActor.h"
#include "CelestialObject.h"
#include "CelestialPlayer.h"
#include "OverviewPlayer.h"
#include "CelestialBody.h"
#include "NiagaraComponent.h"
#include "EngineUtils.h"
#include "Planet.h"


ACelestialGameMode::ACelestialGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACelestialGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// Make widgets
	OverviewWidget = CreateWidget<UUserWidget, APlayerController>(GetWorld()->GetFirstPlayerController(), OverviewWidgetClass);		// Create widget
	CelestialWidget = CreateWidget<UUserWidget, APlayerController>(GetWorld()->GetFirstPlayerController(), CelestialWidgetClass);		// Create widget

	PC = GetWorld()->GetFirstPlayerController();

	SetPerspective(1);

	// Gets all ACelestialBodies and adds them to bodies
	for (TActorIterator<ACelestialBody> Itr(GetWorld()); Itr; ++Itr) {
		bodies.Add(*Itr);
	}

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
	
	if (currentPerspective == 1)
	{
		for (int i = 0; i < celestialObjects.Num(); i++) {
			ICelestialObject* thisObject = celestialObjects[i];

			thisObject->UpdateVelocity(bodies, DeltaTime);
			thisObject->UpdatePosition(DeltaTime);
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
		PC->Possess(Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AOverviewPlayer::StaticClass())));
		CelestialWidget->RemoveFromViewport();
		OverviewWidget->AddToViewport(0);
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
		currentPerspective = perspective;
		AOrbitDebugActor::Get()->DrawOrbits();

		TArray<AActor*> NiagaraSystems;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), UNiagaraComponent::StaticClass(), NiagaraSystems);
		for (auto& System : NiagaraSystems)
		{
			Cast<UNiagaraComponent>(System)->SetPaused(true);
		}
		break;
	}
	case 1: // Celestial mode
	{
		PC->Possess(Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACelestialPlayer::StaticClass())));
		OverviewWidget->RemoveFromViewport();
		CelestialWidget->AddToViewport(0);
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		currentPerspective = perspective;
		AOrbitDebugActor::Get()->ClearOrbits();

		TArray<AActor*> NiagaraSystems;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), UNiagaraComponent::StaticClass(), NiagaraSystems);
		for (auto& System : NiagaraSystems)
		{
			Cast<UNiagaraComponent>(System)->SetPaused(true);
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

