// This is a copyright notice


#include "CelestialGameMode.h"
//#include "GameFramework/Actor.h"
#include "OrbitDebugActor.h"
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
}

void ACelestialGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (AOrbitDebugActor::Get()->DrawType != EDrawType::Ribbon && AOrbitDebugActor::Get()->bAutoDraw)
	{
		AOrbitDebugActor::Get()->DrawOrbits();
	}
	

	for (int i = 0; i < bodies.Num(); i++) {
		ACelestialBody* thisBody = bodies[i];

		thisBody->UpdateVelocity(bodies, DeltaTime);
		thisBody->UpdatePosition(DeltaTime);
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

