// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Helpers/MinMax.h"

/**
 * 
 */

class INoiseFilter;
class UShapeSettings;

// Copy of one noise layer's settings, taken on the game thread in FShapeGenerator::UpdateSettings
struct FNoiseLayerSnapshot
{
	// Owned by FShapeGenerator. Null if the layer had no noise settings
	INoiseFilter* Filter = nullptr;
	bool bEnabled = true;
	bool bFirstLayerAsMask = false;
};

class MINISOLARSYSTEM_API FShapeGenerator
{
public:
	FShapeGenerator();
	~FShapeGenerator();
	
	// Game thread only, and only while no generation tasks are running
	void UpdateSettings(UShapeSettings* NewSettings);

	UShapeSettings* Settings = nullptr;

	// Safe to call from worker threads: reads only the snapshot taken in UpdateSettings, never the settings UObjects
	float CalculateUnscaledElevation(FVector PointOnUnitSphere) const;

	float GetScaledElevation(float unscaledElevation) const;

	MinMax ElevationMinMax;

private:
	void DeleteNoiseFilters();

	// Snapshot of Settings, so worker threads never read UObjects the UI can change mid-generation
	TArray<FNoiseLayerSnapshot> Layers;
	float Radius = 0.f;
};
