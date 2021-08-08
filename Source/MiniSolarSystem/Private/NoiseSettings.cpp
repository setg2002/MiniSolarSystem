// Copyright Soren Gilbertson


#include "NoiseSettings.h"

UNoiseSettings::UNoiseSettings()
{
}

UNoiseSettings::~UNoiseSettings()
{
}

void UNoiseSettings::SetFilterType(EFilterType NewFilterType)
{
	NoiseSettings.FilterType = NewFilterType;
	OnSettingsAssetChanged.Broadcast();
}

void UNoiseSettings::SetSimpleNoiseSettings(FSimpleNoiseSettings NewSettings)
{
	NoiseSettings.SimpleNoiseSettings = NewSettings;
	OnSettingsAssetChanged.Broadcast();
}

void UNoiseSettings::SetRidgidNoiseSettings(FRidgidNoiseSettings NewSettings)
{
	NoiseSettings.RidgidNoiseSettings = NewSettings;
	OnSettingsAssetChanged.Broadcast();
}
