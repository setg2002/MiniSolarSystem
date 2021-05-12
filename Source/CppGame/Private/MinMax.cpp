// Copyright Soren Gilbertson


#include "MinMax.h"

MinMax::MinMax()
{
	Min = TNumericLimits<float>::Max();
	Max = TNumericLimits<float>::Min();
}

MinMax::~MinMax()
{
}

void MinMax::AddValue(float v)
{
	if (v > Max)
	{
		Max = v;
	}
	if (v < Min)
	{
		Min = v;
	}
}

