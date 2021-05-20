// Copyright Soren Gilbertson


#include "ShapeGenerator.h"
#include "ShapeSettings.h"
#include "INoiseFilter.h"
#include "NoiseFilterFactory.h"
#include "MinMax.h"


ShapeGenerator::ShapeGenerator()
{
}

void ShapeGenerator::UpdateSettings(UShapeSettings* settings)
{
	this->Settings = settings;
	NoiseFilters.SetNum(Settings->GetNoiseLayers().Num());
	for (int i = 0; i < Settings->GetNoiseLayers().Num(); i++)
	{
		NoiseFilters[i] = NoiseFilterFactory::CreateNoiseFilter(Settings->GetNoiseLayers()[i]->NoiseSettings);
	}
	ElevationMinMax = new MinMax();
}

ShapeGenerator::~ShapeGenerator()
{
}

float ShapeGenerator::CalculateUnscaledElevation(FVector PointOnUnitSphere)
{
	if (Settings != nullptr)
	{
		float firstLayerValue = 0;
		float elevation = 0;

		if (NoiseFilters.Num() > 0)
		{
			firstLayerValue = NoiseFilters[0]->Evaluate(PointOnUnitSphere);
			if (Settings->GetNoiseLayers()[0]->GetEnabled())
			{
				elevation = firstLayerValue;
			}

			if (NoiseFilters.Num() > 1)
			{
				for (int i = 1; i < NoiseFilters.Num(); i++)
				{
					if (Settings->GetNoiseLayers()[i]->GetEnabled())
					{
						float mask = (Settings->GetNoiseLayers()[i]->GetFirstLayerAsMask()) ? firstLayerValue : 1;
						float newElevation = elevation + NoiseFilters[i]->Evaluate(PointOnUnitSphere) * mask;
						// Only use first layer noise for ocean shading
						if (elevation + newElevation > 0)
						{
							elevation += newElevation;
						}
					}
				}
			}
		}

		ElevationMinMax->AddValue(elevation);
		return elevation;
	}
	else
	{
		return 0;
	}
}

float ShapeGenerator::GetScaledElevation(float unscaledElevation)
{
	float elevation = FMath::Max<float>(0, unscaledElevation);
	elevation = Settings->GetRadius() * (1 + elevation);
	return elevation;
}
