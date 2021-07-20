// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SettingsAsset.generated.h"

/**
 * 
 */
UCLASS()
class CPPGAME_API USettingsAsset : public UObject
{
	GENERATED_BODY()
	
	DECLARE_MULTICAST_DELEGATE(FSettingsAssetChanged);

public:
	USettingsAsset();

	FSettingsAssetChanged OnSettingsAssetChanged;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FName Name;
};
