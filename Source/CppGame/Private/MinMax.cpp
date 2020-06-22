// This is a copyright notice


#include "MinMax.h"

MinMax::MinMax()
{
	Min = 99999999999999999;
	Max = -99999999999999999;
}

MinMax::~MinMax()
{
}

float MinMax::getMin()
{
	return Min;
}

float MinMax::getMax()
{
	return Max;
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

