// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EFilterType.h"
#include "NoiseSettings.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSimpleNoiseSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float Strength = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float BaseRoughness = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float Roughness = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1", ClampMax = "8"))
	int numLayers = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "2"))
	float Persistence = .5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float MinValue;
};

USTRUCT(BlueprintType)
struct FRidgidNoiseSettings : public FSimpleNoiseSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeightMultiplier = .8f;
};

UCLASS(BlueprintType)
class CPPGAME_API UNoiseSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UNoiseSettings();
	~UNoiseSettings();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EFilterType> FilterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "FilterType == 0", EditConditionHides))
	FSimpleNoiseSettings SimpleNoiseSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "FilterType == 1", EditConditionHides))
	FRidgidNoiseSettings RidgidNoiseSettings;
};
