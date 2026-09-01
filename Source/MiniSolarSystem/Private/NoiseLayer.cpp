// Copyright Soren Gilbertson


#include "Shape/NoiseLayer.h"

void UNoiseLayer::SetEnabled(bool NewEnabled)
{
	NoiseLayer.Enabled = NewEnabled;
	OnSettingsAssetChanged.Broadcast();
}

void UNoiseLayer::SetFirstLayerAsMask(bool NewFirstLayerAsMask)
{
	NoiseLayer.UseFirstLayerAsMask = NewFirstLayerAsMask;
	OnSettingsAssetChanged.Broadcast();
}
