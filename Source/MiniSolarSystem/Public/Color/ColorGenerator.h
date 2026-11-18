// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"

class UCurveLinearColor;
class UTexture2D;
class AActor;
class MinMax;
class UColorSettings;
class INoiseFilter;


/**
 * 
 */
class MINISOLARSYSTEM_API FTerrestrialColorGenerator
{
public:
	FTerrestrialColorGenerator();
	void UpdateSettings(UColorSettings* colorSettings);
	~FTerrestrialColorGenerator();

	UColorSettings* ColorSettings = nullptr;

	INoiseFilter* BiomeNoiseFilter = nullptr;

	void UpdateElevation(const MinMax& elevationMinMax);

	// Safe to call from worker threads: reads only the snapshot taken in UpdateSettings, never the settings UObjects
	float BiomePercentFromPoint(FVector PointOnUnitSphere) const;

	void UpdateColors();

	UTexture2D* CreateTexture(TArray<UCurveLinearColor*> Gradients);

private:
	const int32 TextureResolution = 256;

	// Snapshot of the biome settings used by BiomePercentFromPoint, taken on the game thread in UpdateSettings
	TArray<float> BiomeStartHeights;
	float BiomeBlendAmount = 0.f;
	float BiomeNoiseOffset = 0.f;
	float BiomeNoiseStrength = 0.f;
};
