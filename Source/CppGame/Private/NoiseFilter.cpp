// This is a copyright notice


#include "NoiseFilter.h"

NoiseFilter::NoiseFilter(UNoiseSettings* settings)
{
	this->settings = settings;
}

NoiseFilter::~NoiseFilter()
{
}

float NoiseFilter::Evaluate(FVector point)
{
	float noiseValue = 0;
	float frequency = settings->BaseRoughness;
	float amplitude = 1;

	for (int i = 0; i < settings->numLayers; i++)
	{
		float v = FMath::PerlinNoise3D(point * frequency + settings->Center);
		noiseValue += (v + 1) * .5f * amplitude;
		frequency *= settings->Roughness;
		amplitude *= settings->Persistence;
	}

	noiseValue = FMath::Max<float>(0, noiseValue - settings->MinValue);
	return noiseValue * settings->Strength;
}
