// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "ColorCurveType.h"
#include "UObject/Interface.h"
#include "ColorCurveReplacedEvent.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UColorCurveReplacedEvent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINISOLARSYSTEM_API IColorCurveReplacedEvent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RegisterOnGradientReplaced(const FOnGradientChangedCallback& Callback);
	
};
