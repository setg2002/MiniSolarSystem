// This is a copyright notice


#include "CelestialGameMode.h"
#include "OrbitDebugActor.h"
#include "CelestialBody.h"
#include "EngineUtils.h"


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

	AOrbitDebugActor::Get()->DrawOrbits();

	for (int i = 0; i < bodies.Num(); i++) {
		ACelestialBody* thisBody = bodies[i];

		thisBody->UpdateVelocity(bodies, DeltaTime);
		thisBody->UpdatePosition(DeltaTime);
	}
}

