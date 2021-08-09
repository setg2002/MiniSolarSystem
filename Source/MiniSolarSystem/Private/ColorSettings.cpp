// Copyright Soren Gilbertson


#include "ColorSettings.h"

UColorSettings::UColorSettings()
{
	PlanetMat = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Materials/Instances/M_Planet_Inst.M_Planet_Inst'"), NULL, LOAD_None, NULL);
}

void UBiome::SetStartHeight(float NewHeight)
{
	Biome.StartHeight = NewHeight;
	OnSettingsAssetChanged.Broadcast();
}

void UBiome::SetGradient(UCurveLinearColor* NewGradient)
{
	Biome.Gradient = NewGradient;
	OnSettingsAssetChanged.Broadcast();
}


void UBiomeColorSettings::SetUsingNoise(bool NewUsingNoise)
{
	BiomeColorSettings.bUsingNoise = NewUsingNoise;
	OnSettingsAssetChanged.Broadcast();
}

void UBiomeColorSettings::SetNoiseOffset(float NewNoiseOffset)
{
	BiomeColorSettings.NoiseOffset = NewNoiseOffset;
	OnSettingsAssetChanged.Broadcast();
}

void UBiomeColorSettings::SetNoiseStrength(float NewNoiseStrength)
{
	BiomeColorSettings.NoiseStrength = NewNoiseStrength;
	OnSettingsAssetChanged.Broadcast();
}

void UBiomeColorSettings::AddBiome(UBiome* NewBiome)
{
	BiomeColorSettings.Biomes.Add(NewBiome);
	OnSettingsAssetChanged.Broadcast();
}

void UBiomeColorSettings::RemoveBiome(int32 index)
{
	BiomeColorSettings.Biomes.RemoveAt(index);
	OnSettingsAssetChanged.Broadcast();
}

void UBiomeColorSettings::SetBlendAmount(float NewBlendAmount)
{
	BiomeColorSettings.blendAmount = NewBlendAmount;
	OnSettingsAssetChanged.Broadcast();
}

void UBiomeColorSettings::SetNoise(UNoiseSettings* NewNoise)
{
	BiomeColorSettings.Noise = NewNoise;
	OnSettingsAssetChanged.Broadcast();
}


void UColorSettings::SetBiomeColorSettings(UBiomeColorSettings* NewColorSettings)
{
	ColorSettings.BiomeColorSettings = NewColorSettings;
	OnSettingsAssetChanged.Broadcast();
}

void UColorSettings::SetTintPercent(float NewTintPercent)
{
	ColorSettings.BiomeTintPercent = NewTintPercent;
	OnSettingsAssetChanged.Broadcast();
}

void UColorSettings::SetOceanColor(UCurveLinearColor* NewOceanColor)
{
	ColorSettings.OceanColor = NewOceanColor;
	OnSettingsAssetChanged.Broadcast();
}
