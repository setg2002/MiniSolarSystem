// This is a copyright notice


#include "ShapeGenerator.h"

ShapeGenerator::ShapeGenerator(UShapeSettings* settings)
{
	this->Settings = settings;
}

ShapeGenerator::~ShapeGenerator()
{
}

FVector ShapeGenerator::CalculatePointOnPlanet(FVector PointOnUnitSphere)
{
	if (Settings != nullptr)
	{
		FVector point = PointOnUnitSphere * Settings->PlanetRadius;
		return point;
	}
	else
	{
		return FVector().ZeroVector;
	}
}

