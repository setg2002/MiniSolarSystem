// Copyright Soren Gilbertson


#include "NoiseLayer.h"

void UNoiseLayer::SetEnabled(bool NewEnabled)
{
	Enabled = NewEnabled;
	OnNoiseLayerChanged.Broadcast();
}

void UNoiseLayer::SetFirstLayerAsMask(bool NewFirstLayerAsMask)
{
	UseFirstLayerAsMask = NewFirstLayerAsMask;
	OnNoiseLayerChanged.Broadcast();
}
