// Copyright Soren Gilbertson


#include "CelestialGameInstance.h"
#include "LoadingScreen.h"

void UCelestialGameInstance::Init()
{
	LoadConfig();
	SetVolume(Volume);
}

float UCelestialGameInstance::GetCurrentMaxFPS()
{
	return GEngine->GetMaxFPS();
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

void UCelestialGameInstance::SetVolume_Implementation(float NewVolume) 
{ 
	Volume = NewVolume; 
	SaveConfig(); 
}
