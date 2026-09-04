// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CelestialSaveGame.generated.h"

/**
 * 
 */

USTRUCT()
struct FBodySystemRecord
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FName Name;

	UPROPERTY(SaveGame)
	TArray<uint32> SystemIDs;

	// Default Values
	FBodySystemRecord()
	{
		Name = NAME_None;
	}
};

USTRUCT()
struct FAssetRecord
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	UClass* Class;

	UPROPERTY(SaveGame)
	FName Name;

	UPROPERTY(SaveGame)
	TArray<uint8> AssetData;

	// Default Values
	FAssetRecord()
	{
		Class = nullptr;
		Name = NAME_None;
	}
};

USTRUCT()
struct FDiscAssetRecord : public FAssetRecord
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FSoftObjectPath ObjectPath;
};

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
struct FComponentRecord : public FActorRecord
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FName ParentName;

	// Default Values
	FComponentRecord()
	{
		ParentName = NAME_None;
		Class = nullptr;
		Transform = FTransform();
		Name = NAME_None;
	}
};

USTRUCT()
struct FAsteroidRecord
{
	GENERATED_BODY()
	
	UPROPERTY(SaveGame)
	int32 SpawnCount;
	
	UPROPERTY(SaveGame)
	int32 Height;
	
	UPROPERTY(SaveGame)
	int32 Radius;
	
	UPROPERTY(SaveGame)
	int32 Width;

	UPROPERTY(SaveGame)
	FTransform Transform;
	
	FAsteroidRecord()
		: SpawnCount(0), Height(0), Radius(0), Width(0)
	{
	}
};


UCLASS()
class MINISOLARSYSTEM_API UCelestialSaveGame : public USaveGame
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

	UPROPERTY(VisibleAnywhere)
	float GravConst;

	UPROPERTY(VisibleAnywhere)
	int32 AsteroidFieldNum;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FAsteroidRecord> AsteroidBeltData;

	UPROPERTY(VisibleAnywhere)
	TArray<FBodySystemRecord> BodySystemsData;

	// ~~~ Asset Saving ~~~

	UPROPERTY(VisibleAnywhere)
	TArray<FAssetRecord> GradientAssets;

	UPROPERTY(VisibleAnywhere)
	TArray<FAssetRecord> SettingsAssets;

	UPROPERTY(VisibleAnywhere)
	TArray<FDiscAssetRecord> OnDiscSettingsAssets;
};
