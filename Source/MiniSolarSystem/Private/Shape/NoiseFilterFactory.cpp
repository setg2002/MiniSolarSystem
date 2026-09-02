// Copyright Soren Gilbertson


#include "Shape/NoiseFilterFactory.h"
#include "Shape/SimpleNoiseFilter.h"
#include "Shape/RidgidNoiseFilter.h"

INoiseFilter* NoiseFilterFactory::CreateNoiseFilter(UNoiseSettings* NoiseSettings)
{
	ensure(NoiseSettings);
	switch (NoiseSettings->GetFilterType())
	{
	case Smooth:
		return new SimpleNoiseFilter(NoiseSettings->GetSimpleNoiseSettings());
	case Rigid:
		return new RigidNoiseFilter(NoiseSettings->GetRigidNoiseSettings());
	default:
		return new SimpleNoiseFilter(NoiseSettings->GetSimpleNoiseSettings());;
	}
}