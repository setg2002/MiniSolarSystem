// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "SettingsAsset.h"
#include "ColorSettings.generated.h"

/**
 * 
 */

class UNoiseSettings;

USTRUCT(BlueprintType)
struct FBiome_
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float StartHeight = 0;
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadOnly)
	UCurveLinearColor* Gradient = nullptr;
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	FColor Tint = FColor::White;
};

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBiomeHeightUpdated);

UCLASS(BlueprintType)
class UBiome : public USettingsAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(SaveGame, EditAnywhere)
	FBiome_ Biome;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FBiomeHeightUpdated OnBiomeHeightUpdated;

	FBiome_ GetStruct() const { return Biome; }
	bool SetStruct(FBiome_ NewStruct)
	{
		Biome = NewStruct;
		return true;
	}

	UFUNCTION()
	void BiomeUpdated() { OnSettingsAssetChanged.Broadcast(); }

	UFUNCTION(BlueprintCallable)
	float GetStartHeight() const { return Biome.StartHeight; }
	UFUNCTION(BlueprintCallable)
	void SetStartHeight(float NewHeight);

	UFUNCTION(BlueprintCallable)
	UCurveLinearColor* GetGradient() const { return Biome.Gradient; }
	UFUNCTION(BlueprintCallable)
	void SetGradient(UCurveLinearColor* NewGradient);
};


USTRUCT(BlueprintType)
struct FBiomeColorSettings_
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, EditAnywhere, meta = (InlineEditConditionToggle))
	bool bUsingNoise = false;

	UPROPERTY(SaveGame, EditAnywhere, meta = (EditCondition = "bUsingNoise"))
	float NoiseOffset = 0;

	UPROPERTY(SaveGame, EditAnywhere, meta = (EditCondition = "bUsingNoise"))
	float NoiseStrength = 0;

	UPROPERTY(EditAnywhere)
	TArray<UBiome*> Biomes = {};

	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float blendAmount = 0;

	UPROPERTY(SaveGame, EditAnywhere, meta = (EditCondition = "bUsingNoise"))
	UNoiseSettings* Noise = nullptr;
};

UCLASS()
class UBiomeColorSettings : public USettingsAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(SaveGame, EditAnywhere)
	FBiomeColorSettings_ BiomeColorSettings;

public:
	UBiomeColorSettings();

	FBiomeColorSettings_ GetStruct() const { return BiomeColorSettings; }
	bool SetStruct(FBiomeColorSettings_ NewStruct)
	{
		BiomeColorSettings = NewStruct;
		for (UBiome* Biome : BiomeColorSettings.Biomes)
		{
			Biome->OnBiomeHeightUpdated.AddDynamic(this, &UBiomeColorSettings::SortBiomesByHeight);
		}
		return true;
	}

	UFUNCTION(BlueprintCallable)
	bool GetUsingNoise() const { return BiomeColorSettings.bUsingNoise; }
	UFUNCTION(BlueprintCallable)
	void SetUsingNoise(bool NewUsingNoise);

	UFUNCTION(BlueprintCallable)
	float GetNoiseOffset() const { return BiomeColorSettings.NoiseOffset; }
	UFUNCTION(BlueprintCallable)
	void SetNoiseOffset(float NewNoiseOffset);

	UFUNCTION(BlueprintCallable)
	float GetNoiseStrength() const { return BiomeColorSettings.NoiseStrength; }
	UFUNCTION(BlueprintCallable)
	void SetNoiseStrength(float NewNoiseStrength);

	UFUNCTION(BlueprintCallable)
	TArray<UBiome*> GetBiomes() const { return BiomeColorSettings.Biomes; }
	UFUNCTION(BlueprintCallable)
	void AddBiome(UBiome* NewBiome);
	UFUNCTION(BlueprintCallable)
	void RemoveBiome(int32 index);
	UFUNCTION(BlueprintCallable)
	void RemoveBiomeByRef(UBiome* ref);
	UFUNCTION(BlueprintCallable)
	void RemoveAllBiomes();
	UFUNCTION(BlueprintCallable)
	void SortBiomesByHeight();

	UFUNCTION(BlueprintCallable)
	float GetBlendAmount() const { return BiomeColorSettings.blendAmount; }
	UFUNCTION(BlueprintCallable)
	void SetBlendAmount(float NewBlendAmount);

	UFUNCTION(BlueprintCallable)
	UNoiseSettings* GetNoise() const { return BiomeColorSettings.Noise; }
	UFUNCTION(BlueprintCallable)
	void SetNoise(UNoiseSettings* NewNoise);
};


USTRUCT(BlueprintType)
struct FColorSettings_
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float BiomeTintPercent = 0;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadOnly)
	UCurveLinearColor* OceanColor = nullptr;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	UBiomeColorSettings* BiomeColorSettings = nullptr;
};

UCLASS()
class MINISOLARSYSTEM_API UColorSettings : public USettingsAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(SaveGame, EditAnywhere)
	FColorSettings_ ColorSettings;

	UFUNCTION()
	void ColorSettingsUpdated() { OnSettingsAssetChanged.Broadcast(); }

public:
	UColorSettings();

	FColorSettings_ GetStruct() const { return ColorSettings; }
	bool SetStruct(FColorSettings_ NewStruct)
	{
		ColorSettings = NewStruct;
		return true;
	}

	void Init();

	UMaterialInterface* PlanetMat;

	UPROPERTY(BlueprintReadOnly)
	UMaterialInstanceDynamic* DynamicMaterial;

	UFUNCTION(BlueprintCallable)
	UBiomeColorSettings* GetBiomeColorSettings() const { return ColorSettings.BiomeColorSettings; }
	UFUNCTION(BlueprintCallable)
	void SetBiomeColorSettings(UBiomeColorSettings* NewColorSettings);

	UFUNCTION(BlueprintCallable)
	UCurveLinearColor* GetOceanColor() const { return ColorSettings.OceanColor; }
	UFUNCTION(BlueprintCallable)
	void SetOceanColor(UCurveLinearColor* NewOceanColor);

	UFUNCTION(BlueprintCallable)
	float GetTintPercent() const { return ColorSettings.BiomeTintPercent; }
	UFUNCTION(BlueprintCallable)
	void SetTintPercent(float NewTintPercent);
};
