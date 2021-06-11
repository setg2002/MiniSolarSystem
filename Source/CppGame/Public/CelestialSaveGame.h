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
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	UClass* Class;

	UPROPERTY(SaveGame)
	FTransform Transform;

	UPROPERTY(SaveGame)
	FName Name;

	UPROPERTY(SaveGame)
	TArray<uint8> ActorData;

	// Default Values
	FActorRecord()
	{
		Class = nullptr;
		Transform = FTransform();
		Name = NAME_None;
	}
};

USTRUCT()
struct FComponentRecord
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FName ParentName;

	UPROPERTY(SaveGame)
	UClass* Class;

	UPROPERTY(SaveGame)
	FTransform Transform;

	UPROPERTY(SaveGame)
	FName Name;

	UPROPERTY(SaveGame)
	TArray<uint8> ActorData;

	// Default Values
	FComponentRecord()
	{
		ParentName = NAME_None;
		Class = nullptr;
		Transform = FTransform();
		Name = NAME_None;
	}
};


UCLASS()
class CPPGAME_API UCelestialSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UCelestialSaveGame();

	UPROPERTY(VisibleAnywhere)
	TArray<FActorRecord> CelestialBodyData;

	UPROPERTY(VisibleAnywhere)
	TArray<FComponentRecord> CelestialComponentData;

	UPROPERTY(VisibleAnywhere)
	FActorRecord OrbitVisualizationData;

	UPROPERTY(VisibleAnywhere)
	FActorRecord CelestialPlayerData;

	UPROPERTY(VisibleAnywhere)
	FActorRecord OverviewPlayerData;
	UPROPERTY(VisibleAnywhere)
	float OverviewArmLength;
	UPROPERTY(VisibleAnywhere)
	FRotator OverviewCameraRotation;
};
