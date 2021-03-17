// This is a copyright notice


#include "SimpleNoiseFilter.h"
#include "G:\UnrealProjects\CppGame\Plugins\SimplexNoise\Source\SimplexNoise\Public\SimplexNoiseBPLibrary.h"

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
		FVector noiseVector = FVector(point * frequency + Settings.Center);
		float v = USimplexNoiseBPLibrary::SimplexNoise3D(noiseVector.X, noiseVector.Y, noiseVector.Z);
		noiseValue += (v + 1) * .5f * amplitude;
		frequency *= Settings.Roughness;
		amplitude *= Settings.Persistence;
	}

	noiseValue = noiseValue - Settings.MinValue;
	return noiseValue * Settings.Strength;
}
