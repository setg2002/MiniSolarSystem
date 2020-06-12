// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NoiseSettings.generated.h"

/**
 * 
 */

UCLASS(EditInlineNew)
class CPPGAME_API UNoiseSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UNoiseSettings();
	~UNoiseSettings();

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float Strength = 1;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float BaseRoughness = 1;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float Roughness = 2;
	UPROPERTY(EditAnywhere)
	FVector Center;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "8"))
	int numLayers = 1;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "5"))
	float Persistence = .5f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MinValue;
};
