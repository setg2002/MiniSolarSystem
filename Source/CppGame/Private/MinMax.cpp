// This is a copyright notice


#include "MinMax.h"

MinMax::MinMax()
{
	Min = 999999999999999999;
	Max = -999999999999999999;
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

