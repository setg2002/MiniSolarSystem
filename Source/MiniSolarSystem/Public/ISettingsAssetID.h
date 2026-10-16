// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "ISettingsAssetID.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSettingsAssetChanged);
DECLARE_DYNAMIC_DELEGATE(FSettingsAssetChangedCallback);

/**
 * 
 */
UINTERFACE(MinimalAPI)
class USettingsAssetID : public UInterface
{
	GENERATED_BODY()
};

class MINISOLARSYSTEM_API ISettingsAssetID
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual TArray<uint32> GetAppliedIDs() const { return TArray<uint32>(); }

	UFUNCTION()
	virtual uint32 GetID() const { return 0; }
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RegisterOnSettingsAssetChanged(const FSettingsAssetChangedCallback& Callback);
};
