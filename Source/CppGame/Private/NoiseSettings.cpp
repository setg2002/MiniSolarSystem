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
	FilterType = NewFilterType;
	OnNoiseSettingsChanged.Broadcast();
}

void UNoiseSettings::SetSimpleNoiseSettings(FSimpleNoiseSettings NewSettings)
{
	SimpleNoiseSettings = NewSettings;
	OnNoiseSettingsChanged.Broadcast();
}

void UNoiseSettings::SetRidgidNoiseSettings(FRidgidNoiseSettings NewSettings)
{
	RidgidNoiseSettings = NewSettings;
	OnNoiseSettingsChanged.Broadcast();
}
