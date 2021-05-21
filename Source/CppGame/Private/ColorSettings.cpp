// Copyright Soren Gilbertson


#include "ColorSettings.h"

UColorSettings::UColorSettings()
{
	PlanetMat = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/Instances/M_Planet_Inst.M_Planet_Inst'"), NULL, LOAD_None, NULL);
}

void UBiome::SetStartHeight(float NewHeight)
{
	StartHeight = NewHeight;
	OnBiomeChanged.Broadcast();
}

void UBiomeColorSettings::SetUsingNoise(bool NewUsingNoise)
{
	bUsingNoise = NewUsingNoise;
	OnBiomeColorSettingsChanged.Broadcast();
}

void UBiomeColorSettings::SetNoiseOffset(float NewNoiseOffset)
{
	NoiseOffset = NewNoiseOffset;
	OnBiomeColorSettingsChanged.Broadcast();
}
	 
void UBiomeColorSettings::SetNoiseStrength(float NewNoiseStrength)
{
	NoiseStrength = NewNoiseStrength;
	OnBiomeColorSettingsChanged.Broadcast();
}
	 
void UBiomeColorSettings::AddBiome(UBiome* NewBiome)
{
	Biomes.Add(NewBiome);
	OnBiomeColorSettingsChanged.Broadcast();
}
	 
void UBiomeColorSettings::RemoveBiome(int32 index)
{
	Biomes.RemoveAt(index);
	OnBiomeColorSettingsChanged.Broadcast();
}
	 
void UBiomeColorSettings::SetBlendAmount(float NewBlendAmount)
{
	blendAmount = NewBlendAmount;
	OnBiomeColorSettingsChanged.Broadcast();
}

void UBiomeColorSettings::SetNoise(UNoiseSettings* NewNoise)
{
	Noise = NewNoise;
	OnBiomeColorSettingsChanged.Broadcast();
}

void UColorSettings::SetTintPercent(float NewTintPercent)
{
	BiomeTintPercent = NewTintPercent;
	OnColorSettingsChanged.Broadcast();
}
