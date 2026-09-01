// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API UUIBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Velocity")
	static FVector GetRelativeVelocity(class ACelestialPlayer* Player, class ACelestialBody* Body);
};
