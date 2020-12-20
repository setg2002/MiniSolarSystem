// This is a copyright notice


#include "NoiseFilterFactory.h"
#include "SimpleNoiseFilter.h"
#include "RidgidNoiseFIlter.h"

INoiseFilter* NoiseFilterFactory::CreateNoiseFilter(UNoiseSettings* NoiseSettings)
{
	ensure(NoiseSettings);
	switch (NoiseSettings->FilterType)
	{
	case Smooth:
		return new SimpleNoiseFilter(NoiseSettings->SimpleNoiseSettings);
	case Ridgid:
		return new RidgidNoiseFilter(NoiseSettings->RidgidNoiseSettings);
	default:
		return new SimpleNoiseFilter(NoiseSettings->SimpleNoiseSettings);;
	}
}