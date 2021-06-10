// Copyright Soren Gilbertson

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
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"), BlueprintReadWrite)
	int radius = 50;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"), BlueprintReadWrite)
	int mass = 1000;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"), BlueprintReadWrite)
	int luminosity = 50;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	FColor color = FColor(128, 0, 0);

	FORCEINLINE bool operator==(const FStarProperties& other) const
	{
		return other.radius == radius && other.mass == mass && other.luminosity == luminosity && other.color == color;
	}

	FORCEINLINE bool operator!=(const FStarProperties& other) const
	{
		return !(*this == other);
	}
};
