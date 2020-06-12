// This is a copyright notice


#include "ShapeGenerator.h"

ShapeGenerator::ShapeGenerator(UShapeSettings* settings)
{
	this->Settings = settings;
	noiseFilter = new NoiseFilter(Settings->noiseSettings);
}

ShapeGenerator::~ShapeGenerator()
{
}

FVector ShapeGenerator::CalculatePointOnPlanet(FVector PointOnUnitSphere)
{
	if (Settings != nullptr)
	{
		float elevation = noiseFilter->Evaluate(PointOnUnitSphere);
		return PointOnUnitSphere * Settings->PlanetRadius * (1 + elevation);
	}
	else
	{
		return FVector().ZeroVector;
	}
}

