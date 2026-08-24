// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MSSBlueprintFunctionLibrary.generated.h"

class ACelestialGameState;

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API UMSSBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(WorldContext="WorldContextObject"))
	static ACelestialGameState* GetCelestialGameState(UObject* WorldContextObject);
};
