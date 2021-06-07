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
	OnNoiseSettingsChanged.Broadcast();
}

void UNoiseSettings::SetSimpleNoiseSettings(FSimpleNoiseSettings NewSettings)
{
	NoiseSettings.SimpleNoiseSettings = NewSettings;
	OnNoiseSettingsChanged.Broadcast();
}

void UNoiseSettings::SetRidgidNoiseSettings(FRidgidNoiseSettings NewSettings)
{
	NoiseSettings.RidgidNoiseSettings = NewSettings;
	OnNoiseSettingsChanged.Broadcast();
}
