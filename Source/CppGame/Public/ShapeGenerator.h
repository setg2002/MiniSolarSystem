// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "ShapeSettings.h"

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
};
