// Copyright Soren Gilbertson

#pragma once

#include "INoiseFilter.generated.h"

/**
 * 
 */
UINTERFACE()
class MINISOLARSYSTEM_API UNoiseFilter : public UInterface
{
	GENERATED_BODY()
};

class INoiseFilter
{
	GENERATED_BODY()

public:
	virtual float Evaluate(FVector point);
};
