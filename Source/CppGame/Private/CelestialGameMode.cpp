// This is a copyright notice


#include "CelestialGameMode.h"
#include "CelestialBody.h"
#include "EngineUtils.h"


ACelestialGameMode::ACelestialGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACelestialGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ACelestialBody> Itr(GetWorld()); Itr; ++Itr) {
		bodies.Add(*Itr);
		//*Itr->gameMode = *this;
	}
}

// Called every frame
void ACelestialGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < bodies.Num(); i++) {
		ACelestialBody* thisBody = bodies[i];

		thisBody->UpdateVelocity(bodies, DeltaTime);
		thisBody->UpdatePosition(DeltaTime);
	}
}