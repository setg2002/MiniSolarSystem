// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "INoiseFilter.h"
#include "NoiseSettings.h"

/**
 * 
 */

class MINISOLARSYSTEM_API SimpleNoiseFilter : public INoiseFilter
{
public:
	SimpleNoiseFilter(FSimpleNoiseSettings settings);
	~SimpleNoiseFilter();

	FSimpleNoiseSettings Settings;

	virtual float Evaluate(FVector point) override;
};
