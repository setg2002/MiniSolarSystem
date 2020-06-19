// This is a copyright notice

#pragma once

#include "INoiseFilter.generated.h"

/**
 * 
 */
UINTERFACE()
class CPPGAME_API UNoiseFilter : public UInterface
{
	GENERATED_BODY()
};

class INoiseFilter
{
	GENERATED_BODY()

public:
	virtual float Evaluate(FVector point);
};
