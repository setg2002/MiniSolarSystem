// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "ColorSettings.h"
#include "MinMax.h"

/**
 * 
 */
class CPPGAME_API ColorGenerator
{
public:
	void UpdateSettings(UColorSettings* colorSettings);
	~ColorGenerator();

	UColorSettings* ColorSettings;

	const int TextureResolution = 256;

	void UpdateElevation(MinMax* elevationMinMax);

	float BiomePercentFromPoint(FVector PointOnUnitSphere);

	void UpdateColors();
};
