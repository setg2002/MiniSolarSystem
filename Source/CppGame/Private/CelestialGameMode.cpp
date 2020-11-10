// This is a copyright notice


#include "CelestialGameMode.h"
#include "CelestialBody.h"
#include "Planet.h"
#include "Star.h"
#include "G:\UESource\Engine\Source\Runtime\Engine\Classes\Materials\MaterialInstanceDynamic.h"
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

	for (int i = 0; i < bodies.Num(); i++) {
		ACelestialBody* thisBody = bodies[i];

		thisBody->UpdateVelocity(bodies, DeltaTime);
		thisBody->UpdatePosition(DeltaTime);
	}
}

/*void ACelestialGameMode::GenerateDistantStars()
{
	if (distantStars.Num() < numDistantStars)
	{
		for (int i = distantStars.Num(); i < numDistantStars; i++)
		{
			FVector spawnLocation = FVector::ZeroVector;
			float dist = FVector::Dist(spawnLocation, FVector(0, 0, 0));
			while (dist > maxSpawnRange || dist < minSpawnRange)
			{
				spawnLocation.X = FMath::FRandRange(-maxSpawnRange, maxSpawnRange);
				spawnLocation.Y = FMath::FRandRange(-maxSpawnRange, maxSpawnRange);
				spawnLocation.Z = FMath::FRandRange(-maxSpawnRange, maxSpawnRange);
				dist = FVector::Dist(spawnLocation, FVector(0, 0, 0));
			}

			FActorSpawnParameters spawnInfo;
			distantStars.Add(GetWorld()->SpawnActor<AStar>(distantStarBP, spawnLocation, FRotator(0, 0, 0), spawnInfo));
			int randColor = FMath::FRandRange(0, 9);
			distantStars.Last()->starProperties.color = FColor(possibleColors[randColor].X, possibleColors[randColor].Y, possibleColors[randColor].Z);
			distantStars.Last()->UpdateColor();

			float newRadius = FMath::FRandRange(1, 20);
			distantStars.Last()->sphere->SetRelativeScale3D(FVector(newRadius, newRadius, newRadius));
		}
	}
	else
	{
		while (distantStars.Num() > numDistantStars)
		{
			distantStars[distantStars.Num() - 1]->Destroy();
			distantStars.Remove(distantStars.Last());
		}
	}
}*/
