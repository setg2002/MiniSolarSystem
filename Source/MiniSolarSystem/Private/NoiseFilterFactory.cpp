// Copyright Soren Gilbertson


#include "NoiseFilterFactory.h"
#include "SimpleNoiseFilter.h"
#include "RidgidNoiseFilter.h"

INoiseFilter* NoiseFilterFactory::CreateNoiseFilter(UNoiseSettings* NoiseSettings)
{
	ensure(NoiseSettings);
	switch (NoiseSettings->GetFilterType())
	{
	case Smooth:
		return new SimpleNoiseFilter(NoiseSettings->GetSimpleNoiseSettings());
	case Ridgid:
		return new RidgidNoiseFilter(NoiseSettings->GetRidgidNoiseSettings());
	default:
		return new SimpleNoiseFilter(NoiseSettings->GetSimpleNoiseSettings());;
	}
}