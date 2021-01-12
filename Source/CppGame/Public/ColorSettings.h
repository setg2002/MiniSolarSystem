// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NoiseSettings.h"
#include "ColorSettings.generated.h"

/**
 * 
 */


UCLASS()
class UBiome : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		UCurveLinearColor* Gradient;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor Tint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1"))
		float StartHeight;
};

UCLASS()
class UBiomeColorSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (InlineEditConditionToggle))
	bool bUsingNoise;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Instanced, meta = (EditCondition = "bUsingNoise"))
	UNoiseSettings* Noise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUsingNoise"))
	float NoiseOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUsingNoise"))
	float NoiseStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBiome*> Biomes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1"))
	float blendAmount;
};

UCLASS()
class CPPGAME_API UColorSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UColorSettings();

	UPROPERTY(EditAnywhere)
	UCurveLinearColor* OceanColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* PlanetMat;

	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1"))
	float BiomeTintPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBiomeColorSettings* BiomeColorSettings;
};
