// Copyright Soren Gilbertson


#include "ColorSettings.h"

UColorSettings::UColorSettings()
{
	PlanetMat = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/Instances/M_Planet_Inst.M_Planet_Inst'"), NULL, LOAD_None, NULL);
}

void UBiome::SetStartHeight(float NewHeight)
{
	Biome.StartHeight = NewHeight;
	OnBiomeChanged.Broadcast();
}

void UBiome::SetGradient(UCurveLinearColor* NewGradient)
{
	Biome.Gradient = NewGradient;
	OnBiomeChanged.Broadcast();
}

void UBiomeColorSettings::SetUsingNoise(bool NewUsingNoise)
{
	BiomeColorSettings.bUsingNoise = NewUsingNoise;
	OnBiomeColorSettingsChanged.Broadcast();
}

void UBiomeColorSettings::SetNoiseOffset(float NewNoiseOffset)
{
	BiomeColorSettings.NoiseOffset = NewNoiseOffset;
	OnBiomeColorSettingsChanged.Broadcast();
}

void UBiomeColorSettings::SetNoiseStrength(float NewNoiseStrength)
{
	BiomeColorSettings.NoiseStrength = NewNoiseStrength;
	OnBiomeColorSettingsChanged.Broadcast();
}

void UBiomeColorSettings::AddBiome(UBiome* NewBiome)
{
	BiomeColorSettings.Biomes.Add(NewBiome);
	OnBiomeColorSettingsChanged.Broadcast();
}

void UBiomeColorSettings::RemoveBiome(int32 index)
{
	BiomeColorSettings.Biomes.RemoveAt(index);
	OnBiomeColorSettingsChanged.Broadcast();
}

void UBiomeColorSettings::SetBlendAmount(float NewBlendAmount)
{
	BiomeColorSettings.blendAmount = NewBlendAmount;
	OnBiomeColorSettingsChanged.Broadcast();
}

void UBiomeColorSettings::SetNoise(UNoiseSettings* NewNoise)
{
	BiomeColorSettings.Noise = NewNoise;
	OnBiomeColorSettingsChanged.Broadcast();
}

void UColorSettings::SetBiomeColorSettings(UBiomeColorSettings* NewColorSettings)
{
	ColorSettings.BiomeColorSettings = NewColorSettings;
	OnColorSettingsChanged.Broadcast();
}

void UColorSettings::SetTintPercent(float NewTintPercent)
{
	ColorSettings.BiomeTintPercent = NewTintPercent;
	OnColorSettingsChanged.Broadcast();
}

void UColorSettings::SetOceanColor(UCurveLinearColor* NewOceanColor)
{
	ColorSettings.OceanColor = NewOceanColor;
	OnColorSettingsChanged.Broadcast();
}
