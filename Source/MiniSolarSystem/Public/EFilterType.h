// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EFilterType.generated.h"

/**
 * 
 */
 UENUM(BlueprintType)
 enum EFilterType
 {
	 Smooth UMETA(DisplayName = "Simple"),
	 Ridgid UMETA(DisplayName = "Ridgid")
 };
