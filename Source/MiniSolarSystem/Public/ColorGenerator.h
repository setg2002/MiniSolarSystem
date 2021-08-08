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
	TerrestrialColorGenerator(AActor* owner);
	void UpdateSettings(UColorSettings* colorSettings);
	~TerrestrialColorGenerator();

	AActor* Owner;

	UColorSettings* ColorSettings;

	INoiseFilter* BiomeNoiseFilter;

	const int TextureResolution = 256;

	void UpdateElevation(MinMax* elevationMinMax);

	float BiomePercentFromPoint(FVector PointOnUnitSphere);

	void UpdateColors();

	UTexture2D* CreateTexture(FString TextureName, TArray<UCurveLinearColor*> Gradients);
};
