// Copyright Soren Gilbertson


#include "ShapeSettings.h"
#include "Planet.h"

bool UShapeSettings::IsNoiseLayers()
{
	bool exists = true;
	for (int i = 0; i < NoiseLayers.Num(); i++)
	{
		if (NoiseLayers[i]->NoiseSettings == nullptr)
		{
			exists = false;
		}
	}
	return exists;
}

void UShapeSettings::SetRadius(float NewRadius)
{
	PlanetRadius = NewRadius;
	OnShapeSettingsChanged.ExecuteIfBound();
}

void UShapeSettings::AddNoiseLayer(UNoiseLayer* NewNoiseLayer)
{
	NoiseLayers.Add(NewNoiseLayer);
	OnShapeSettingsChanged.ExecuteIfBound();
}

void UShapeSettings::RemoveNoiseLayer(int32 index)
{
	NoiseLayers.RemoveAt(index);
	OnShapeSettingsChanged.ExecuteIfBound();
}
