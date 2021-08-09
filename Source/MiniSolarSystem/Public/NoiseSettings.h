// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "EFilterType.h"
#include "SettingsAsset.h"
#include "NoiseSettings.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSimpleNoiseSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float Strength = 1;
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float BaseRoughness = 1;
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float Roughness = 2;
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	FVector Center;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1", ClampMax = "8"))
	int numLayers = 1;
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "2"))
	float Persistence = .5f;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float MinValue;
};

USTRUCT(BlueprintType)
struct FRidgidNoiseSettings : public FSimpleNoiseSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	float WeightMultiplier = .8f;
};

USTRUCT(BlueprintType)
struct FNoiseSettings_
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, EditAnywhere)
	TEnumAsByte<EFilterType> FilterType;

	UPROPERTY(SaveGame, EditAnywhere, meta = (EditCondition = "FilterType == 0", EditConditionHides))
	FSimpleNoiseSettings SimpleNoiseSettings;
	UPROPERTY(SaveGame, EditAnywhere, meta = (EditCondition = "FilterType == 1", EditConditionHides))
	FRidgidNoiseSettings RidgidNoiseSettings;

};

UCLASS(BlueprintType)
class MINISOLARSYSTEM_API UNoiseSettings : public USettingsAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(SaveGame, EditAnywhere)
	FNoiseSettings_ NoiseSettings;

public:
	FNoiseSettings_ GetStruct() const { return NoiseSettings; }
	bool SetStruct(FNoiseSettings_ NewStruct)
	{
		NoiseSettings = NewStruct;
		return true;
	}

	UNoiseSettings();
	~UNoiseSettings();

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EFilterType> GetFilterType() const { return NoiseSettings.FilterType; }
	UFUNCTION(BlueprintCallable)
	void SetFilterType(EFilterType NewFilterType);

	UFUNCTION(BlueprintCallable)
	FSimpleNoiseSettings GetSimpleNoiseSettings() const { return NoiseSettings.SimpleNoiseSettings; }
	UFUNCTION(BlueprintCallable)
	FRidgidNoiseSettings GetRidgidNoiseSettings() const { return NoiseSettings.RidgidNoiseSettings; }

	UFUNCTION(BlueprintCallable)
	void SetSimpleNoiseSettings(FSimpleNoiseSettings NewSettings);
	UFUNCTION(BlueprintCallable)
	void SetRidgidNoiseSettings(FRidgidNoiseSettings NewSettings);
};