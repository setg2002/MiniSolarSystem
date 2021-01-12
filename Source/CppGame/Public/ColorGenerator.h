// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "MinMax.h"
#include "INoiseFilter.h"
#include "ColorSettings.h"
#include "GameFramework/Actor.h"


/**
 * 
 */
class CPPGAME_API ColorGenerator
{
public:
	ColorGenerator(AActor* owner);
	void UpdateSettings(UColorSettings* colorSettings);
	~ColorGenerator();

	AActor* Owner;

	UColorSettings* ColorSettings;

	INoiseFilter* BiomeNoiseFilter;

	const int TextureResolution = 256;

	void UpdateElevation(MinMax* elevationMinMax);

	float BiomePercentFromPoint(FVector PointOnUnitSphere);

	void UpdateColors();

	UTexture2D* CreateTexture(FString TextureName, TArray<UCurveLinearColor*> Gradients);
};
