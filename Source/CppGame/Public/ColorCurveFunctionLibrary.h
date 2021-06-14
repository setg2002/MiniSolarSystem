// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ColorCurveFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CPPGAME_API UColorCurveFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Texture")
	static UTexture2D* TextureFromCurve(UCurveLinearColor* Gradient, int32 sizeX, int32 sizeY);
};
