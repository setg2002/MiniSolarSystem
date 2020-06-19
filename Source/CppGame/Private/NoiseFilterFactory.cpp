// This is a copyright notice


#include "NoiseFilterFactory.h"
#include "SimpleNoiseFilter.h"
#include "RidgidNoiseFIlter.h"


INoiseFilter* UNoiseFilterFactory::CreateNoiseFilter(UNoiseSettings* NoiseSettings)
{
	switch (NoiseSettings->FilterType)
	{
	case Simple:
		return new SimpleNoiseFilter(NoiseSettings->SimpleNoiseSettings);
	case Ridgid:
		return new RidgidNoiseFilter(NoiseSettings->RidgidNoiseSettings);
	default:
		return new SimpleNoiseFilter(NoiseSettings->SimpleNoiseSettings);;
	}
}
