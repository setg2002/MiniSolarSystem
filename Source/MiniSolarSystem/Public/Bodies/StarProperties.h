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
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	float radius = 50;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 mass = 1000;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 luminosity = 50;

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
	
	void Randomize()
	{
		RandomizeShape();
		RandomizeColor();
	}
	
	void RandomizeShape()
	{
		radius = FMath::InterpEaseIn<float>(5.f, 500.f, FMath::FRand(), 6.f); // Prefer smaller radii
		mass = radius * FMath::RandRange(10, 30);
	}
	
	void RandomizeColor()
	{
		luminosity = FMath::RandRange(10, 100);
		color = FColor::MakeRandomColor();
	}
};
