// Copyright Soren Gilbertson


#include "ShapeSettings.h"
#include "NoiseLayer.h"
#include "Planet.h"


bool UShapeSettings::IsNoiseLayers()
{
	bool exists = true;
	for (int i = 0; i < ShapeSettings.NoiseLayers.Num(); i++)
	{
		if (ShapeSettings.NoiseLayers[i]->NoiseSettings == nullptr)
		{
			exists = false;
		}
	}
	return exists;
}

void UShapeSettings::SetRadius(float NewRadius)
{
	ShapeSettings.PlanetRadius = NewRadius;
	OnSettingsAssetChanged.Broadcast();
}

void UShapeSettings::AddNoiseLayer(UNoiseLayer* NewNoiseLayer)
{
	ShapeSettings.NoiseLayers.Add(NewNoiseLayer);
	OnSettingsAssetChanged.Broadcast();
}

void UShapeSettings::RemoveNoiseLayer(int32 index)
{
	ShapeSettings.NoiseLayers.RemoveAt(index);
	OnSettingsAssetChanged.Broadcast();
}
