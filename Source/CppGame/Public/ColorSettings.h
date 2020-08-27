// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NoiseSettings.h"
#include "ColorSettings.generated.h"

/**
 * 
 */


UCLASS(DefaultToInstanced, EditInlineNew)
class UBiome : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly)
		UCurveLinearColor* Gradient;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor Tint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1"))
		float StartHeight;
};

UCLASS(DefaultToInstanced, EditInlineNew)
class UBiomeColorSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (InlineEditConditionToggle))
	bool bUsingNoise;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Instanced, meta = (EditCondition = "bUsingNoise"))
	UNoiseSettings* Noise;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoiseOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoiseStrength;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Instanced)
	TArray<UBiome*> Biomes;
};

UCLASS(DefaultToInstanced, EditInlineNew)
class CPPGAME_API UColorSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UColorSettings();

	UPROPERTY(Instanced)
	UCurveLinearColorAtlas* Atlas;

	UPROPERTY(EditAnywhere)
	UCurveLinearColorAtlas* OceanAtlas;

	UPROPERTY(EditAnywhere)
	UCurveLinearColorAtlas* BiomeTintAtlas;

	UPROPERTY(EditAnywhere)
	UCurveLinearColor* OceanColor;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UMaterialInterface* PlanetMat;

	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1"))
	float BiomeTintPercent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Instanced)
	UBiomeColorSettings* BiomeColorSettings;
};
