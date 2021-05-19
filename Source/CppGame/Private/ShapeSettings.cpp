// Copyright Soren Gilbertson


#include "ShapeSettings.h"
#include "Planet.h"

bool UShapeSettings::GetNoiseLayers()
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
