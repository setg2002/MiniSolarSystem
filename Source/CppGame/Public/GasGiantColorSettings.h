// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GasGiantColorSettings.generated.h"

/**
 * 
 */

// Forward Declarations
class UNoiseSettings;


UCLASS()
class CPPGAME_API UGasGiantColorSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UGasGiantColorSettings();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BasePlanetMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveLinearColor* Gradient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNoiseSettings* Noise;

	UMaterialInstanceDynamic* DynamicMaterial;
};
