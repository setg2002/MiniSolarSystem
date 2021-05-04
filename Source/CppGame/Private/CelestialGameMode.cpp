// This is a copyright notice


#include "CelestialGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "OrbitDebugActor.h"
#include "CelestialObject.h"
#include "CelestialBody.h"
#include "EngineUtils.h"
#include "Planet.h"


ACelestialGameMode::ACelestialGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACelestialGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ACelestialBody> Itr(GetWorld()); Itr; ++Itr) {
		bodies.Add(*Itr);
	}

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
	
	if (AOrbitDebugActor::Get()->DrawType != EDrawType::Ribbon && AOrbitDebugActor::Get()->bAutoDraw)
	{
		AOrbitDebugActor::Get()->DrawOrbits();
	}
	

	for (int i = 0; i < celestialObjects.Num(); i++) {
		ICelestialObject* thisObject = celestialObjects[i];

		thisObject->UpdateVelocity(bodies, DeltaTime);
		thisObject->UpdatePosition(DeltaTime);
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

