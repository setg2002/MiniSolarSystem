// Copyright Soren Gilbertson


#include "NoiseLayer.h"

UNoiseLayer::UNoiseLayer()
{
	if (Name == NAME_None)
	{
		Name = FName(this->GetName());
	}
}

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
