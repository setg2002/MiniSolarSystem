// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CelestialGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API UCelestialGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	float GetCurrentMaxFPS();

public:
	UFUNCTION(BlueprintCallable)
	static void PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime);
	
	UFUNCTION(BlueprintCallable)
	static void StopLoadingScreen();

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetVolume(float NewVolume);
	UFUNCTION(BlueprintCallable)
	float GetVolume() const { return Volume; }
protected:
	UPROPERTY(BlueprintReadWrite)
	float Volume = 1;
};
