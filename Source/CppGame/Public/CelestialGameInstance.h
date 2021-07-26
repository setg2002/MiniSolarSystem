// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CelestialGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CPPGAME_API UCelestialGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentMaxFPS();
};
