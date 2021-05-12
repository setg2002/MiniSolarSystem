// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CPPGAME_API MinMax
{
public:
	MinMax();
	~MinMax();

	float Min;
	float Max;

	void AddValue(float v);
};
