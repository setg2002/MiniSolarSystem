// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "NoiseSettings.h"
#include "SettingsAsset.h"
#include "NoiseLayer.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FNoiseLayer_
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, EditAnywhere)
	bool Enabled = true;
	UPROPERTY(SaveGame, EditAnywhere)
	bool UseFirstLayerAsMask = false;

};

UCLASS(BlueprintType)
class CPPGAME_API UNoiseLayer : public USettingsAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(SaveGame, EditAnywhere)
	FNoiseLayer_ NoiseLayer;

public:
	FNoiseLayer_ GetStruct() const { return NoiseLayer; }
	bool SetStruct(FNoiseLayer_ NewStruct)
	{
		NoiseLayer = NewStruct;
		return true;
	}

	UFUNCTION(BlueprintCallable)
	bool GetEnabled() const { return NoiseLayer.Enabled; }
	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool NewEnabled);

	UFUNCTION(BlueprintCallable)
	bool GetFirstLayerAsMask() const { return NoiseLayer.UseFirstLayerAsMask; }
	UFUNCTION(BlueprintCallable)
	void SetFirstLayerAsMask(bool NewFirstLayerAsMask);

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	UNoiseSettings* NoiseSettings;
};
