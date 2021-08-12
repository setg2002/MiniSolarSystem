// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "SettingsAsset.h"
#include "ShapeSettings.generated.h"

class UNoiseLayer;

/**
 * 
 */

USTRUCT(BlueprintType)
struct FShapeSettings_
{
	GENERATED_BODY()

public:
	// Planet radius in cm
	UPROPERTY(SaveGame, EditAnywhere)
	float PlanetRadius = 100;

	UPROPERTY(EditAnywhere)
	TArray<UNoiseLayer*> NoiseLayers;

};

UCLASS(BlueprintType)
class MINISOLARSYSTEM_API UShapeSettings : public USettingsAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(SaveGame, EditAnywhere)
	FShapeSettings_ ShapeSettings;

public:
	bool IsNoiseLayers();

	FShapeSettings_ GetStruct() const { return ShapeSettings; }
	bool SetStruct (FShapeSettings_ NewStruct)
	{
		ShapeSettings = NewStruct;
		return true;
	}

	UFUNCTION(BlueprintCallable)
	float GetRadius() const { return ShapeSettings.PlanetRadius; }
	UFUNCTION(BlueprintCallable)
	void SetRadius(float NewRadius);

	UFUNCTION(BlueprintCallable)
	TArray<UNoiseLayer*> GetNoiseLayers() const { return ShapeSettings.NoiseLayers; }
	UFUNCTION(BlueprintCallable)
	void AddNoiseLayer(UNoiseLayer* NewNoiseLayer);
	UFUNCTION(BlueprintCallable)
	void RemoveNoiseLayer(int32 index);

};
