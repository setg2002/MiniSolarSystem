// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "ISettingsAssetID.h"
#include "UObject/NoExportTypes.h"
#include "SettingsAsset.generated.h"

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API USettingsAsset : public UObject, public ISettingsAssetID
{
	GENERATED_BODY()
	
	DECLARE_MULTICAST_DELEGATE(FSettingsAssetChanged);

protected:
	// This is an aray of the IDs of assets that this asset is applied to
	UPROPERTY(SaveGame)
	TArray<uint32> IDs;

	UPROPERTY(SaveGame)
	uint32 ID;

public:
	UFUNCTION()
	virtual TArray<uint32> GetAppliedIDs() const override { return IDs; } 

	UFUNCTION()
	virtual uint32 GetID() const override { return ID; } 

	void AddAppliedID(uint32 NewID);
	void RemoveAppliedID(uint32 IDToRemove);

public:
	USettingsAsset();

	FSettingsAssetChanged OnSettingsAssetChanged;

	UFUNCTION(BlueprintCallable)
	FName GetName();

	UFUNCTION(BlueprintCallable)
	bool SetName(FString NewName);
};
