// Copyright Soren Gilbertson


#include "Helpers/MinMax.h"

MinMax::MinMax()
{
	Min = TNumericLimits<float>::Max();
	Max = TNumericLimits<float>::Lowest();
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

void MinMax::Merge(const MinMax& Other)
{
	Min = FMath::Min(Min, Other.Min); 
	Max = FMath::Max(Max, Other.Max);
}

void MinMax::Reset()
{
	*this = MinMax();
}

