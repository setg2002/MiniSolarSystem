// Copyright Soren Gilbertson


#include "Game/CelestialGameState.h"

#include "Player/CelestialPlayer.h"
#include "Player/OverviewPlayer.h"
#include "Kismet/GameplayStatics.h"


void ACelestialGameState::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	ensure(World);
	
	CelestialPlayer = Cast<ACelestialPlayer>(UGameplayStatics::GetActorOfClass(World, ACelestialPlayer::StaticClass()));
	OverviewPlayer = Cast<AOverviewPlayer>(UGameplayStatics::GetActorOfClass(World, AOverviewPlayer::StaticClass()));
}
