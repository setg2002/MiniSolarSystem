// This is a copyright notice


#include "SimpleNoiseFilter.h"

SimpleNoiseFilter::SimpleNoiseFilter(FSimpleNoiseSettings settings)
{
	this->Settings = settings;
}

SimpleNoiseFilter::~SimpleNoiseFilter()
{
}

float SimpleNoiseFilter::Evaluate(FVector point)
{
	float noiseValue = 0;
	float frequency = Settings.BaseRoughness;
	float amplitude = 1;

	for (int i = 0; i < Settings.numLayers; i++)
	{
		float v = FMath::PerlinNoise3D(point * frequency + Settings.Center);
		noiseValue += (v + 1) * .5f * amplitude;
		frequency *= Settings.Roughness;
		amplitude *= Settings.Persistence;
	}

	noiseValue = FMath::Max<float>(0, noiseValue - Settings.MinValue);
	return noiseValue * Settings.Strength;
}
