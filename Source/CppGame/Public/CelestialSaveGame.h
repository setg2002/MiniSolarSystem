// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CelestialSaveGame.generated.h"

/**
 * 
 */
USTRUCT()
struct FActorRecord
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(SaveGame)
	UClass* Class;

	UPROPERTY(SaveGame)
	FTransform Transform;

	UPROPERTY(SaveGame)
	FName Name;

	UPROPERTY(SaveGame)
	TArray<uint8> ActorData;
};


UCLASS()
class CPPGAME_API UCelestialSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UCelestialSaveGame();

	UPROPERTY(VisibleAnywhere)
	TMap<FName, class ACelestialBody*> CelestialBodies;

	TArray<FActorRecord> CelestialBodyData;

	UPROPERTY(VisibleAnywhere)
	class ACelestialPlayer* CelestialPlayerData;

	UPROPERTY(VisibleAnywhere)
	class AOverviewPlayer* OverviewPlayerData;

	UPROPERTY(VisibleAnywhere)
	class AOrbitDebugActor* OrbitVisualizationData;

};
