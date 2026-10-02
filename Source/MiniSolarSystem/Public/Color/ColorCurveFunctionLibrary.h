// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ColorCurveFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API UColorCurveFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Texture")
	static UTexture2D* TextureFromCurve(UCurveLinearColor* Gradient, int32 sizeX, int32 sizeY);

	UFUNCTION(BlueprintCallable, Category = "Texture")
	static UCurveLinearColor* CreateNewCurve(FName Name, TArray<uint8> Data);
	
	/**
	  * Creates a random gradient curve
	  * 
	  * @param	NumPoints			The number of different colors the generated gradient will have
	  * @param	bClampColorDelta	When true, the hue delta between each color will be clamped to MaxDelta
	  * @param  MaxDelta			The maximum hue delta between each color (0 - 255)
	  */
	UFUNCTION(BlueprintCallable, Category = "Gradiant")
	static UCurveLinearColor* CreateRandomCurve(int32 NumPoints, bool bClampColorDelta, uint8 MaxDelta = 24);
};
