// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "INoiseFilter.h"
#include "NoiseSettings.h"

/**
 * 
 */

class MINISOLARSYSTEM_API RigidNoiseFilter : public INoiseFilter
{
public:
	RigidNoiseFilter(FRigidNoiseSettings settings);
	~RigidNoiseFilter();

	FRigidNoiseSettings Settings;

	virtual float Evaluate(FVector point) override;
};
