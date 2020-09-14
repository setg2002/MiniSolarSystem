// This is a copyright notice


#include "RidgidNoiseFilter.h"
#include "G:\UnrealProjects\CppGame\Plugins\SimplexNoise-SimplexNoise.1.2.0_UE4.25.3\SimplexNoise-SimplexNoise.1.2.0_UE4.25.3\Source\SimplexNoise\Public\SimplexNoiseBPLibrary.h"

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
		FVector noiseVector = FVector(point * frequency + Settings.Center);
		float v = 1 - FMath::Abs(USimplexNoiseBPLibrary::SimplexNoise3D(noiseVector.X, noiseVector.Y, noiseVector.Z));
		v *= v;
		v *= weight;
		weight = v * Settings.WeightMultiplier;
		noiseValue += v * amplitude;
		frequency *= Settings.Roughness;
		amplitude *= Settings.Persistence;
	}

	noiseValue = noiseValue - Settings.MinValue;
	return noiseValue * Settings.Strength;
}
