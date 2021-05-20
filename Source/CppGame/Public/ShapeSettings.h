// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NoiseLayer.h"
#include "ShapeSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CPPGAME_API UShapeSettings : public UDataAsset
{
	GENERATED_BODY()

	DECLARE_DELEGATE(FShapeSettingsChanged);

private:
	// Planet radius in cm
	UPROPERTY(EditAnywhere)
	float PlanetRadius = 100;

	UPROPERTY(EditAnywhere)
	TArray<UNoiseLayer*> NoiseLayers;

public:
	bool IsNoiseLayers();

	FShapeSettingsChanged OnShapeSettingsChanged;

	UFUNCTION(BlueprintCallable)
	float GetRadius() const { return PlanetRadius; }
	UFUNCTION(BlueprintCallable)
	void SetRadius(float NewRadius);

	UFUNCTION(BlueprintCallable)
	TArray<UNoiseLayer*> GetNoiseLayers() const { return NoiseLayers; }
	UFUNCTION(BlueprintCallable)
	void AddNoiseLayer(UNoiseLayer* NewNoiseLayer);
	UFUNCTION(BlueprintCallable)
	void RemoveNoiseLayer(int32 index);
};
