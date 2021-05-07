// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine\DataTable.h"
#include "StarProperties.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FStarProperties : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"), BlueprintReadWrite)
	int radius = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"), BlueprintReadWrite)
	int mass = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"), BlueprintReadWrite)
	int luminosity = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor color = FColor(128, 0, 0);
};
