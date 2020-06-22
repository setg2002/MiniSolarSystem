// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EFilterType.generated.h"

/**
 * 
 */
 UENUM()
 enum EFilterType
 {
	 Smooth  UMETA(DisplayName = "Simple"),
	 Ridgid  UMETA(DisplayName = "Ridgid")
 };
