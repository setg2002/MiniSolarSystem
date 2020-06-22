// This is a copyright notice

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

	float getMin();
	float getMax();

	void AddValue(float v);
};
