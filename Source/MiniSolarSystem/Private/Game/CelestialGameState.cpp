// Copyright Soren Gilbertson


#include "Game/CelestialGameState.h"

#include "Game/CelestialGameMode.h"
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

bool ACelestialGameState::ShouldIgnoreConfirmationID(const FString& ConfirmationID) const
{
	return ConfirmationIDsToIgnore.Contains(ConfirmationID);
}

void ACelestialGameState::AddConfirmationIDToIgnore(const FString& ConfirmationID)
{
	ConfirmationIDsToIgnore.AddUnique(ConfirmationID);
}

void ACelestialGameState::SetConfirmationIDsToIgnore(const TArray<FString>& InConfirmationIDs)
{
	// Only allow overriding of ConfirmationIDsToIgnore array if we are loading into the game where perspective is 128
	if (GetWorld()->GetAuthGameMode<ACelestialGameMode>()->GetCurrentPerspective() == 128) // TODO- find better way of determining loading state
	{
		ConfirmationIDsToIgnore = InConfirmationIDs;
	}
}

void ACelestialGameState::ClearConfirmationIDsToIgnore()
{
	ConfirmationIDsToIgnore.Empty();
}
