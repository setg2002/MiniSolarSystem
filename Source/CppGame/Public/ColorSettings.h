// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ColorSettings.generated.h"

/**
 * 
 */


UCLASS(DefaultToInstanced, EditInlineNew)
class UBiome : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor Tint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1"))
		float StartHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1"))
		float TimtPercent;
};

UCLASS(DefaultToInstanced, EditInlineNew)
class UBiomeColorSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Instanced)
	TArray<UBiome*> Biomes;
};

UCLASS(DefaultToInstanced, EditInlineNew)
class CPPGAME_API UColorSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UColorSettings();

	UPROPERTY(EditInstanceOnly)
	UCurveLinearColor* Curve;

	UPROPERTY(EditInstanceOnly)
	UCurveLinearColorAtlas* Atlas;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UMaterial* PlanetMat;

	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Instanced)
	UBiomeColorSettings* BiomeColorSettings;
};
