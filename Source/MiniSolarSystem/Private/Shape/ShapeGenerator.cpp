// Copyright Soren Gilbertson


#include "Shape/ShapeGenerator.h"
#include "Shape/NoiseFilterFactory.h"
#include "Shape/ShapeSettings.h"
#include "Shape/INoiseFilter.h"
#include "Shape/NoiseLayer.h"
#include "Helpers/MinMax.h"


FShapeGenerator::FShapeGenerator()
{
}

void FShapeGenerator::UpdateSettings(UShapeSettings* settings)
{
	this->Settings = settings;
	
	for (INoiseFilter* Filter : NoiseFilters)
	{
		delete Filter;
	}
	
	NoiseFilters.SetNum(Settings->GetNoiseLayers().Num());
	for (int32 i = 0; i < Settings->GetNoiseLayers().Num(); i++)
	{
		NoiseFilters[i] = NoiseFilterFactory::CreateNoiseFilter(Settings->GetNoiseLayers()[i]->NoiseSettings);
	}
	ElevationMinMax.Reset();
}

FShapeGenerator::~FShapeGenerator()
{
	for (INoiseFilter* Filter : NoiseFilters)
	{
		delete Filter;
	}
}

float FShapeGenerator::CalculateUnscaledElevation(FVector PointOnUnitSphere)
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
		
		return elevation;
	}
	else
	{
		return 0;
	}
}

float FShapeGenerator::GetScaledElevation(float unscaledElevation)
{
	float elevation = FMath::Max<float>(0, unscaledElevation);
	elevation = Settings->GetRadius() * (1 + elevation);
	return elevation;
}
