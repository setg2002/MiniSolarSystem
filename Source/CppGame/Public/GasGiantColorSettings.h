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
class AGasGiant;


UCLASS()
class CPPGAME_API UGasGiantColorSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UGasGiantColorSettings();

	void SetOwner(AGasGiant* owner);

	AGasGiant* Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BasePlanetMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveLinearColor* Gradient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNoiseSettings* Noise;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInstanceDynamic* DynamicMaterial;

	// Voronoi Settings \\

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storm Settings", meta = (ClampMin = "0"))
	int NumStorms = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storm Settings", meta = (ClampMin = "0"))
	float StormFalloff = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Storm Settings", AdvancedDisplay, meta = (ClampMin = "0", ClampMax = "1024"))
	int LowBound = 100;

	UPROPERTY(EditAnywhere, Category = "Storm Settings", AdvancedDisplay, meta = (ClampMin = "0", ClampMax = "1024"))
	int HighBound = 924;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
