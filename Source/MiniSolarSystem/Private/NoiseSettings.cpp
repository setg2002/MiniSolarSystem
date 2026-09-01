// Copyright Soren Gilbertson


#include "Shape/NoiseSettings.h"

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

void UNoiseSettings::SetRigidNoiseSettings(FRigidNoiseSettings NewSettings)
{
	NoiseSettings.RigidNoiseSettings = NewSettings;
	OnSettingsAssetChanged.Broadcast();
}
