// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "INoiseFilter.h"
#include "NoiseSettings.h"

/**
 * 
 */

class MINISOLARSYSTEM_API RidgidNoiseFilter : public INoiseFilter
{
public:
	RidgidNoiseFilter(FRidgidNoiseSettings settings);
	~RidgidNoiseFilter();

	FRidgidNoiseSettings Settings;

	virtual float Evaluate(FVector point) override;
};
