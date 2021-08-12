// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "ISettingsAssetID.generated.h"

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
};
