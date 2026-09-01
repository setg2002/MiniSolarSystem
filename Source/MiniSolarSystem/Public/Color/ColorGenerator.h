// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"

class AActor;
class MinMax;
class UColorSettings;
class INoiseFilter;


/**
 * 
 */
class MINISOLARSYSTEM_API TerrestrialColorGenerator
{
public:
	TerrestrialColorGenerator();
	void UpdateSettings(UColorSettings* colorSettings);
	~TerrestrialColorGenerator();

	UColorSettings* ColorSettings;

	INoiseFilter* BiomeNoiseFilter;

	void UpdateElevation(MinMax* elevationMinMax);

	float BiomePercentFromPoint(FVector PointOnUnitSphere);

	void UpdateColors();

	UTexture2D* CreateTexture(TArray<UCurveLinearColor*> Gradients);

private:
	const int32 TextureResolution = 256;
};
