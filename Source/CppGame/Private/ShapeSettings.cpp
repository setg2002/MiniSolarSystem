// Copyright Soren Gilbertson


#include "ShapeSettings.h"
#include "Planet.h"

FPrimaryAssetId UShapeSettings::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FPrimaryAssetType("ShapeSettings"), FName(this->GetName()));
}

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
	OnShapeSettingsChanged.Broadcast();
}

void UShapeSettings::AddNoiseLayer(UNoiseLayer* NewNoiseLayer)
{
	NoiseLayers.Add(NewNoiseLayer);
	OnShapeSettingsChanged.Broadcast();
}

void UShapeSettings::RemoveNoiseLayer(int32 index)
{
	NoiseLayers.RemoveAt(index);
	OnShapeSettingsChanged.Broadcast();
}
