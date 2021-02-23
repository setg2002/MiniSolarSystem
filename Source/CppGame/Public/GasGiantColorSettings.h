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

	// Voronoi Settings \\

	UPROPERTY(EditAnywhere, Category = "Storm Settings", meta = (ClampMin = "0"))
	int8 NumStorms = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storm Settings", meta = (ClampMin = "0"))
	float StormFalloff = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storm Settings", AdvancedDisplay, meta = (ClampMin = "0", ClampMax = "1024"))
	int LowBound = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storm Settings", AdvancedDisplay, meta = (ClampMin = "0", ClampMax = "1024"))
	int HighBound = 924;

	UFUNCTION(BlueprintCallable, CallInEditor)
	UTexture2D* MakeVoronoiTexture();
};
