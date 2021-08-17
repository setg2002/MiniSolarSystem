// Copyright Soren Gilbertson


#include "CelestialGameInstance.h"
#include "LoadingScreen.h"

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

void UCelestialGameInstance::SetVolume_Implementation(float NewVolume) { ; } // This has to be here otherwise it won't compile
