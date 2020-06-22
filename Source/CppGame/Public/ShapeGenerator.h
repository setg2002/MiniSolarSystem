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
	ShapeGenerator(UShapeSettings* settings);
	~ShapeGenerator();

	UShapeSettings* Settings;

	FVector CalculatePointOnPlanet(FVector PointOnUnitSphere);

	TArray<INoiseFilter*> NoiseFilters;

	MinMax* ElevationMinMax;
};
