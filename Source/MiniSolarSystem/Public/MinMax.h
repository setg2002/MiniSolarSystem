// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MINISOLARSYSTEM_API MinMax
{
public:
	MinMax();
	~MinMax();

	float Min;
	float Max;

	void AddValue(float v);
};