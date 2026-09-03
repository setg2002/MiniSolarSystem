// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MSSMacros.generated.h"


// Delay shortcut
#define LATER_SECS(seconds, ...) \
     FTimerHandle __tempTimerHandle; \
     GetWorldTimerManager().SetTimer(__tempTimerHandle, FTimerDelegate().CreateLambda(__VA_ARGS__), seconds, false);

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API UMSSMacros : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
