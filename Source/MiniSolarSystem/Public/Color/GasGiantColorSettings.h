// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "GasGiantColorSettings.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FGasGiantColorSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	UCurveLinearColor* Gradient = nullptr;

	UPROPERTY(SaveGame, EditAnywhere, Category = "Storm Settings", meta = (ClampMin = "0"))
	int NumStorms = 25;

	UPROPERTY(SaveGame, EditAnywhere, Category = "Storm Settings", meta = (ClampMin = "0"))
	float StormFalloff = 2.5f;
};
