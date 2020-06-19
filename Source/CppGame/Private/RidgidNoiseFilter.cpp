// This is a copyright notice


#include "RidgidNoiseFilter.h"

RidgidNoiseFilter::RidgidNoiseFilter(FRidgidNoiseSettings settings)
{
	this->Settings = settings;
}

RidgidNoiseFilter::~RidgidNoiseFilter()
{
}

float RidgidNoiseFilter::Evaluate(FVector point)
{
	float noiseValue = 0;
	float frequency = Settings.BaseRoughness;
	float amplitude = 1;
	float weight = 1;

	for (int i = 0; i < Settings.numLayers; i++)
	{
		float v = 1 - FMath::Abs(FMath::PerlinNoise3D(point * frequency + Settings.Center));
		v *= v;
		v *= weight;
		weight = FMath::Clamp<float>(v * Settings.WeightMultiplier, 0, 1);
		noiseValue += v * amplitude;
		frequency *= Settings.Roughness;
		amplitude *= Settings.Persistence;
	}

	noiseValue = FMath::Max<float>(0, noiseValue - Settings.MinValue);
	return noiseValue * Settings.Strength;
}
