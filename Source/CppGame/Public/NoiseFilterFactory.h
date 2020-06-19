// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "INoiseFilter.h"
#include "SimpleNoiseFilter.h"
#include "RidgidNoiseFilter.h"
#include "NoiseSettings.h"
#include "NoiseFilterFactory.generated.h"

/**
 * 
 */

class INoiseFilter;
class UNoiseSettings;

UCLASS()
class CPPGAME_API UNoiseFilterFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	static INoiseFilter* CreateNoiseFilter(UNoiseSettings* NoiseSettings);
};
