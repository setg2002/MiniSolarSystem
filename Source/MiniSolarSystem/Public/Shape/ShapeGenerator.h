// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class MinMax;
class INoiseFilter;
class UShapeSettings;

class MINISOLARSYSTEM_API FShapeGenerator
{
public:
	FShapeGenerator();
	void UpdateSettings(UShapeSettings* settings);
	~FShapeGenerator();

	UShapeSettings* Settings;

	float CalculateUnscaledElevation(FVector PointOnUnitSphere);

	float GetScaledElevation(float unscaledElevation);

	TArray<INoiseFilter*> NoiseFilters;

	MinMax* ElevationMinMax;
};
