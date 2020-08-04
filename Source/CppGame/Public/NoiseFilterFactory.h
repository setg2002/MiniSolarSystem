// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "INoiseFilter.h"
#include "SimpleNoiseFilter.h"
#include "RidgidNoiseFilter.h"
#include "NoiseSettings.h"

/**
 * 
 */
class CPPGAME_API NoiseFilterFactory
{
public:
	static INoiseFilter* CreateNoiseFilter(UNoiseSettings* NoiseSettings);
};
