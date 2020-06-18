// This is a copyright notice


#include "ShapeGenerator.h"

ShapeGenerator::ShapeGenerator(UShapeSettings* settings)
{
	this->Settings = settings;
	NoiseFilters.SetNum(Settings->NoiseLayers.Num());
	for (int i = 0; i < Settings->NoiseLayers.Num(); i++)
	{
		ensure(Settings->NoiseLayers[i]->NoiseSettings);
		NoiseFilters[i] = new NoiseFilter(Settings->NoiseLayers[i]->NoiseSettings);
	}
}

ShapeGenerator::~ShapeGenerator()
{
}

FVector ShapeGenerator::CalculatePointOnPlanet(FVector PointOnUnitSphere)
{
	if (Settings != nullptr)
	{
		float firstLayerValue = 0;
		float elevation = 0;

		if (NoiseFilters.Num() > 0)
		{
			firstLayerValue = NoiseFilters[0]->Evaluate(PointOnUnitSphere);
			if (Settings->NoiseLayers[0]->Enabled)
			{
				elevation = firstLayerValue;
			}
		}

		for (int i = 1; i < NoiseFilters.Num(); i++)
		{
			if (Settings->NoiseLayers[i]->Enabled)
			{
				float mask = (Settings->NoiseLayers[i]->UseFirstLayerAsMask) ? firstLayerValue : 1;
				elevation += NoiseFilters[i]->Evaluate(PointOnUnitSphere) * mask;
			}
		}
		return PointOnUnitSphere * Settings->PlanetRadius * (1 + elevation);
	}
	else
	{
		return FVector().ZeroVector;
	}
}

