// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"

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

	UColorSettings* ColorSettings;

	INoiseFilter* BiomeNoiseFilter;

	void UpdateElevation(MinMax* elevationMinMax);

	float BiomePercentFromPoint(FVector PointOnUnitSphere);

	void UpdateColors();

	UTexture2D* CreateTexture(TArray<UCurveLinearColor*> Gradients);

private:
	const int32 TextureResolution = 256;
};
