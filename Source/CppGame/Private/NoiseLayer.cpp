// Copyright Soren Gilbertson


#include "NoiseLayer.h"

void UNoiseLayer::SetEnabled(bool NewEnabled)
{
	NoiseLayer.Enabled = NewEnabled;
	OnNoiseLayerChanged.Broadcast();
}

void UNoiseLayer::SetFirstLayerAsMask(bool NewFirstLayerAsMask)
{
	NoiseLayer.UseFirstLayerAsMask = NewFirstLayerAsMask;
	OnNoiseLayerChanged.Broadcast();
}
