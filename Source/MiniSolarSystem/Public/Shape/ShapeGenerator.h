// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class MinMax;
class INoiseFilter;
class UShapeSettings;

class MINISOLARSYSTEM_API ShapeGenerator
{
public:
	ShapeGenerator();
	void UpdateSettings(UShapeSettings* settings);
	~ShapeGenerator();

	UShapeSettings* Settings;

	float CalculateUnscaledElevation(FVector PointOnUnitSphere);

	float GetScaledElevation(float unscaledElevation);

	TArray<INoiseFilter*> NoiseFilters;

	MinMax* ElevationMinMax;
};
