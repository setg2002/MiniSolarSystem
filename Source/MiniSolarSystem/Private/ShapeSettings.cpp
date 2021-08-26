// Copyright Soren Gilbertson


#include "ShapeSettings.h"
#include "CelestialGameMode.h"
#include "OrbitDebugActor.h"
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
	// This is probably the single worst line in this solution
	AOrbitDebugActor::Get()->UpdateWidthSpecificBody((Cast<ACelestialBody>(Cast<ACelestialGameMode>(AOrbitDebugActor::Get()->GetWorld()->GetAuthGameMode())->GetAssetByID(this->IDs[0]))));
}

void UShapeSettings::AddNoiseLayer(UNoiseLayer* NewNoiseLayer)
{
	ShapeSettings.NoiseLayers.Add(NewNoiseLayer);
	OnSettingsAssetChanged.Broadcast();
}

void UShapeSettings::RemoveNoiseLayer(int32 index)
{
	if (ShapeSettings.NoiseLayers.IsValidIndex(index))
	{
		ShapeSettings.NoiseLayers[index]->RemoveAppliedID(ID);
		ShapeSettings.NoiseLayers.RemoveAt(index);
		OnSettingsAssetChanged.Broadcast();
	}
}

void UShapeSettings::RemoveAllNoiseLayers()
{
	for (uint16 i = 0; i < ShapeSettings.NoiseLayers.Num(); i++)
	{
		RemoveNoiseLayer(0);
	}
}
