// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "ShapeSettings.h"
#include "INoiseFilter.h"
#include "NoiseFilterFactory.h"
#include "MinMax.h"

/**
 * 
 */
class CPPGAME_API ShapeGenerator
{
public:
	void UpdateSettings(UShapeSettings* settings);
	~ShapeGenerator();

	UShapeSettings* Settings;

	float CalculateUnscaledElevation(FVector PointOnUnitSphere);

	float GetScaledElevation(float unscaledElevation);

	TArray<INoiseFilter*> NoiseFilters;

	MinMax* ElevationMinMax;
};
