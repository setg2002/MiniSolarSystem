// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NoiseSettings.h"
#include "ColorSettings.generated.h"

/**
 * 
 */


UCLASS(BlueprintType)
class UBiome : public UDataAsset
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FBiomeChanged);

private:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float StartHeight;

public:
	FBiomeChanged OnBiomeChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveLinearColor* Gradient;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Tint;

	UFUNCTION(BlueprintCallable)
	float GetStartHeight() const { return StartHeight; }
	UFUNCTION(BlueprintCallable)
	void SetStartHeight(float NewHeight);
};

UCLASS()
class UBiomeColorSettings : public UDataAsset
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FBiomeColorSettingsChanged);

private:
	UPROPERTY(EditAnywhere, meta = (InlineEditConditionToggle))
	bool bUsingNoise;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUsingNoise"))
	float NoiseOffset;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUsingNoise"))
	float NoiseStrength;

	UPROPERTY(EditAnywhere)
	TArray<UBiome*> Biomes;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float blendAmount;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUsingNoise"))
	UNoiseSettings* Noise;

public:
	FBiomeColorSettingsChanged OnBiomeColorSettingsChanged;

	UFUNCTION(BlueprintCallable)
	bool GetUsingNoise() const { return bUsingNoise; }
	UFUNCTION(BlueprintCallable)
	void SetUsingNoise(bool NewUsingNoise);

	UFUNCTION(BlueprintCallable)
	float GetNoiseOffset() const { return NoiseOffset; }
	UFUNCTION(BlueprintCallable)
	void SetNoiseOffset(float NewNoiseOffset);

	UFUNCTION(BlueprintCallable)
	float GetNoiseStrength() const { return NoiseStrength; }
	UFUNCTION(BlueprintCallable)
	void SetNoiseStrength(float NewNoiseStrength);

	UFUNCTION(BlueprintCallable)
	TArray<UBiome*> GetBiomes() const { return Biomes; }
	UFUNCTION(BlueprintCallable)
	void AddBiome(UBiome* NewBiome);
	UFUNCTION(BlueprintCallable)
	void RemoveBiome(int32 index);

	UFUNCTION(BlueprintCallable)
	float GetBlendAmount() const { return blendAmount; }
	UFUNCTION(BlueprintCallable)
	void SetBlendAmount(float NewBlendAmount);

	UFUNCTION(BlueprintCallable)
	UNoiseSettings* GetNoise() const { return Noise; }
	UFUNCTION(BlueprintCallable)
	void SetNoise(UNoiseSettings* NewNoise);
};

UCLASS()
class CPPGAME_API UColorSettings : public UDataAsset
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FColorSettingsChanged);
	
private:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float BiomeTintPercent;

public:
	UColorSettings();

	FColorSettingsChanged OnColorSettingsChanged;

	UPROPERTY(EditAnywhere)
	UCurveLinearColor* OceanColor;

	UMaterialInterface* PlanetMat;

	UPROPERTY(BlueprintReadOnly)
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBiomeColorSettings* BiomeColorSettings;

	UFUNCTION(BlueprintCallable)
	float GetTintPercent() const { return BiomeTintPercent; }
	UFUNCTION(BlueprintCallable)
	void SetTintPercent(float NewTintPercent);
};
