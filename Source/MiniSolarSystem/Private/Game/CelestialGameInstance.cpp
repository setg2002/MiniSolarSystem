// Copyright Soren Gilbertson


#include "Game/CelestialGameInstance.h"
#include "LoadingScreen.h"
#include "Shape/NoiseLayer.h"

void UCelestialGameInstance::Init()
{
	LoadConfig();
	SetVolume(Volume);
	SetUIVolume(UIVol);
	SetMaxFPS(MaxFPS);
}

void UCelestialGameInstance::SetMaxFPS_Implementation(int32 NewMaxFPS)
{
	MaxFPS = NewMaxFPS;
	SaveConfig();
}

void UCelestialGameInstance::PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{
	ILoadingScreenModule& LoadingScreenModule = ILoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime);
}

void UCelestialGameInstance::StopLoadingScreen()
{
	ILoadingScreenModule& LoadingScreenModule = ILoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}

void UCelestialGameInstance::CopyNoiseLayer(UNoiseLayer* NoiseLayerToCopy)
{ 
	CopiedNoiseLayer = NoiseLayerToCopy; 
}

void UCelestialGameInstance::PasteNoiseLayer(UNoiseLayer* NoiseLayerToPasteTo)
{
	if (CopiedNoiseLayer)
	{
		NoiseLayerToPasteTo->SetStruct(FNoiseLayer_(CopiedNoiseLayer->GetStruct().Enabled, CopiedNoiseLayer->GetStruct().UseFirstLayerAsMask));
		FNoiseSettings_ StructToCopyFrom = CopiedNoiseLayer->NoiseSettings->GetStruct();
		NoiseLayerToPasteTo->NoiseSettings->SetStruct(
			FNoiseSettings_(
				StructToCopyFrom.FilterType,
				FSimpleNoiseSettings(
					StructToCopyFrom.SimpleNoiseSettings.Strength,
					StructToCopyFrom.SimpleNoiseSettings.BaseRoughness,
					StructToCopyFrom.SimpleNoiseSettings.Roughness,
					StructToCopyFrom.SimpleNoiseSettings.Center,
					StructToCopyFrom.SimpleNoiseSettings.numLayers,
					StructToCopyFrom.SimpleNoiseSettings.Persistence,
					StructToCopyFrom.SimpleNoiseSettings.MinValue
				),
				FRigidNoiseSettings(
					StructToCopyFrom.RigidNoiseSettings.Strength,
					StructToCopyFrom.RigidNoiseSettings.BaseRoughness,
					StructToCopyFrom.RigidNoiseSettings.Roughness,
					StructToCopyFrom.RigidNoiseSettings.Center,
					StructToCopyFrom.RigidNoiseSettings.numLayers,
					StructToCopyFrom.RigidNoiseSettings.Persistence,
					StructToCopyFrom.RigidNoiseSettings.MinValue,
					StructToCopyFrom.RigidNoiseSettings.WeightMultiplier
				)
			)
		);
		NoiseLayerToPasteTo->OnSettingsAssetChanged.Broadcast();
	}
}

void UCelestialGameInstance::SetVolume_Implementation(float NewVolume) 
{ 
	Volume = NewVolume; 
	SaveConfig();
}

void UCelestialGameInstance::SetUIVolume_Implementation(float NewUIVolume)
{
	UIVol = NewUIVolume;
	SaveConfig();
}

void UCelestialGameInstance::SetTutorialSeen(bool TutorialSeen)
{ 
	bTutorialSeen = TutorialSeen; 
	SaveConfig();
}
