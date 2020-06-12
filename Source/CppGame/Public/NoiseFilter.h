// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "NoiseSettings.h"

/**
 * 
 */
class CPPGAME_API NoiseFilter
{
public:
	NoiseFilter(UNoiseSettings* settings);
	~NoiseFilter();

	UNoiseSettings* settings;

	float Evaluate(FVector point);
};
