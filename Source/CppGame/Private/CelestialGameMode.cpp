// This is a copyright notice


#include "CelestialGameMode.h"


ACelestialGameMode::ACelestialGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACelestialGameMode::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<ACelestialBody> bodiesToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), bodiesToFind, bodies);
}

// Called every frame
void ACelestialGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}