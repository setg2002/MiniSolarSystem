// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "MainMenuTextReader.generated.h"

/**
 * 
 */
UCLASS()
class CPPGAME_API UMainMenuTextReader : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) 
	static FText GetText();
};
