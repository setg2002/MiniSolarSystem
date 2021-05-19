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
	OnNoiseSettingsChanged.ExecuteIfBound();
}
