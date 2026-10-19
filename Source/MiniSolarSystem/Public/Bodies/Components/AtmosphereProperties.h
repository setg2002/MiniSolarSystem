// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
//#include "Engine\DataTable.h"
#include "AtmosphereProperties.generated.h"


USTRUCT(BlueprintType)
struct FAtmosphereProperties/* : public FTableRowBase*/
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "0"))
	float Height = 0;
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "0"))
	float Density = 0.15f;
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "0"))
	float g = 0.8f;
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "0"))
	float MieHeightScale = 0.25f;
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "0"))
	float OpacityMultiplier = 1;

	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "1"))
	int8 Steps_i = 16;
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "1"))
	int8 Steps_j = 4;

	UPROPERTY(SaveGame, EditAnywhere)
	FLinearColor Ambient = FLinearColor(0, 0, 0, 0);
	UPROPERTY(SaveGame, EditAnywhere)
	FLinearColor Mie = FLinearColor(.5f, .5f, .5f, .5f);
	UPROPERTY(SaveGame, EditAnywhere)
	FLinearColor Ray = FLinearColor(.333f, .666f, 1, 1);
	
	void RandomizeProperties(const float PlanetRadius = 100.f)
	{
		Height = FMath::RandRange(.8f, 2.f) * PlanetRadius; // Scale height by planet radius
		Density = FMath::Pow(FMath::RandRange(0.f, 1.f), 10);
		g = FMath::RandRange(0.f, 1.f);
		MieHeightScale = FMath::RandRange(0.f, 1.f);
		OpacityMultiplier = FMath::RandRange(0.f, 2.f);
		
		Ambient = FLinearColor(FMath::FRand(), FMath::FRand(), FMath::FRand(), FMath::Pow(FMath::FRand(), 90)); // Fully random color but bias the alpha towards 0 so the color is not overwhelming
		Mie = FLinearColor(FMath::FRand(), FMath::FRand(), FMath::FRand(), FMath::FRand());
		Ray = FLinearColor(FMath::FRand(), FMath::FRand(), FMath::FRand(), 1.f - FMath::Pow(FMath::FRand(), 5)); // Bias the alpha towards 1 so the color can be seen
	}
};
