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

void FShapeGenerator::UpdateSettings(UShapeSettings* NewSettings)
{
	this->Settings = NewSettings;

	DeleteNoiseFilters();
	Layers.Reset();
	Radius = 0.f;

	if (Settings)
	{
		// One copy of the array here instead of one per vertex on the worker threads
		const TArray<UNoiseLayer*>& NoiseLayers = Settings->GetNoiseLayers();

		Layers.SetNum(NoiseLayers.Num());
		for (int32 i = 0; i < NoiseLayers.Num(); i++)
		{
			const UNoiseLayer* NoiseLayer = NoiseLayers[i];
			if (NoiseLayer == nullptr)
			{
				// Keep the slot so layer 0 stays the first layer used for masking; it contributes nothing
				Layers[i].bEnabled = false;
				continue;
			}

			if (NoiseLayer->NoiseSettings)
			{
				Layers[i].Filter = NoiseFilterFactory::CreateNoiseFilter(NoiseLayer->NoiseSettings);
			}
			Layers[i].bEnabled = NoiseLayer->GetEnabled();
			Layers[i].bFirstLayerAsMask = NoiseLayer->GetFirstLayerAsMask();
		}

		Radius = Settings->GetRadius();
	}

	ElevationMinMax.Reset();
}

FShapeGenerator::~FShapeGenerator()
{
	DeleteNoiseFilters();
}

void FShapeGenerator::DeleteNoiseFilters()
{
	for (FNoiseLayerSnapshot& Layer : Layers)
	{
		delete Layer.Filter;
		Layer.Filter = nullptr;
	}
}

float FShapeGenerator::CalculateUnscaledElevation(FVector PointOnUnitSphere) const
{
	if (Layers.Num() == 0)
	{
		return 0;
	}

	const float firstLayerValue = Layers[0].Filter ? Layers[0].Filter->Evaluate(PointOnUnitSphere) : 0.f;
	float elevation = 0;

	if (Layers[0].bEnabled)
	{
		elevation = firstLayerValue;
	}

	for (int32 i = 1; i < Layers.Num(); i++)
	{
		const FNoiseLayerSnapshot& Layer = Layers[i];
		if (Layer.bEnabled && Layer.Filter)
		{
			float mask = Layer.bFirstLayerAsMask ? firstLayerValue : 1;
			float newElevation = elevation + Layer.Filter->Evaluate(PointOnUnitSphere) * mask;
			// Only use first layer noise for ocean shading
			if (elevation + newElevation > 0)
			{
				elevation += newElevation;
			}
		}
	}

	return elevation;
}

float FShapeGenerator::GetScaledElevation(float unscaledElevation) const
{
	float elevation = FMath::Max<float>(0, unscaledElevation);
	elevation = Radius * (1 + elevation);
	return elevation;
}
