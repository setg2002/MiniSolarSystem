// Copyright Soren Gilbertson


#include "Shape/ShapeSettings.h"
#include "Game/CelestialGameMode.h"
#include "OrbitDebugActor.h"
#include "Shape/NoiseLayer.h"
#include "Bodies/Planet.h"
#include "Engine/World.h"


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
	
	if (!IDs.IsEmpty())
	{
		if (AOrbitDebugActor* OrbitDebugActor = AOrbitDebugActor::Get())
		{
			if (UWorld* World = OrbitDebugActor->GetWorld())
			{
				if (ACelestialGameMode* GameMode = Cast<ACelestialGameMode>(World->GetAuthGameMode()))
				{
					if (ACelestialBody* TargetBody = Cast<ACelestialBody>(GameMode->GetAssetByID(IDs[0])))
					{
						OrbitDebugActor->UpdateWidthSpecificBody(TargetBody);
					}
				}
			}
		}
	}
}

void UShapeSettings::AddNoiseLayer(UNoiseLayer* NewNoiseLayer)
{
	ShapeSettings.NoiseLayers.Add(NewNoiseLayer);
	OnSettingsAssetChanged.Broadcast();
}

void UShapeSettings::RemoveNoiseLayer(int32 index, bool bBroadcastChange)
{
	if (ShapeSettings.NoiseLayers.IsValidIndex(index))
	{
		if (UNoiseLayer* NoiseLayer = ShapeSettings.NoiseLayers[index])
		{
			NoiseLayer->RemoveAppliedID(ID);
		}
		ShapeSettings.NoiseLayers.RemoveAt(index);
		
		if (bBroadcastChange)
		{
			OnSettingsAssetChanged.Broadcast();
		}
	}
}

void UShapeSettings::RemoveAllNoiseLayers()
{
	for (int32 i = ShapeSettings.NoiseLayers.Num() - 1; i >= 0; --i)
	{
		RemoveNoiseLayer(i, false);
	}
	OnSettingsAssetChanged.Broadcast(); // Only broadcast change once after all layers are removed
}

void UShapeSettings::RandomizeForPlanet(APlanet* Planet)
{
	RemoveAllNoiseLayers();
	
	ShapeSettings.PlanetRadius = FMath::RandRange(10.f, 500.f);
	
	// TODO implement better system for multiple layers
	int32 RandomNoiseLayerCount = 1;// FMath::RandRange(1, 3);
	for (int32 i = 0; i < RandomNoiseLayerCount; i++)
	{
		UNoiseLayer* NewNoiseLayer = NewObject<UNoiseLayer>();
		ShapeSettings.NoiseLayers.Add(NewNoiseLayer);
		NewNoiseLayer->NoiseSettings = Planet->CreateSettingsAsset<UNoiseSettings>(UNoiseSettings::StaticClass());
		NewNoiseLayer->Randomize();
	}
	OnSettingsAssetChanged.Broadcast();
}
