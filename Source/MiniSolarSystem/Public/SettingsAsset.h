// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "ISettingsAssetID.h"
#include "UObject/NoExportTypes.h"
#include "SettingsAsset.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSettingsAssetChanged);

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API USettingsAsset : public UObject, public ISettingsAssetID
{
	GENERATED_BODY()

protected:
	// This is an aray of the IDs of assets that this asset is applied to
	UPROPERTY(SaveGame)
	TArray<uint32> IDs;

	UPROPERTY(SaveGame)
	uint32 ID;

	// Need this because GetAssetsByPath stores the asset name in FAssetData with the class name not the 
	// actual asset name for some reason
	UPROPERTY(SaveGame)
	FString LocalName = FString("");

public:
	UFUNCTION()
	virtual TArray<uint32> GetAppliedIDs() const override { return IDs; } 

	UFUNCTION()
	virtual uint32 GetID() const override { return ID; } 

	virtual void AddAppliedID(uint32 NewID);
	virtual void RemoveAppliedID(uint32 IDToRemove);

public:
	USettingsAsset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FSettingsAssetChanged OnSettingsAssetChanged;

	UFUNCTION(BlueprintCallable)
	FName GetName();

	UFUNCTION(BlueprintCallable)
	bool SetName(FString NewName);
};
