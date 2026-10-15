// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "ColorCurveType.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGradientChanged, UCurveLinearColor*, NewGradient);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGradientChangedCallback, UCurveLinearColor*, NewGradient);

/**
 * 
 */

UENUM(BlueprintType)
enum EColorCurveType
{
	Ocean,
	Biome,
	GasGiant,
	RingSystem
};

