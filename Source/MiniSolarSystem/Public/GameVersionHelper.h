// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "GameVersion.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameVersionHelper.generated.h"

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API UGameVersionHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable)
	static FORCEINLINE FString GetGameVersionString()
	{
		return GAME_VERSION_STRING;
	}

	UFUNCTION(BlueprintCallable)
	static FORCEINLINE FString GetGameVersionStringShort()
	{
		return GAME_VERSION_STRING_SHORT;
	}
};
