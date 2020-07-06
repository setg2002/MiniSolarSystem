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
	ColorGenerator(UColorSettings* colorSettings);
	~ColorGenerator();

	UColorSettings* ColorSettings;

	const int TextureResolution = 64;

	void UpdateElevation(MinMax* elevationMinMax);

	void UpdateColors();
};
