// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Engine\DataTable.h"
#include "AtmosphereProperties.generated.h"


USTRUCT(BlueprintType)
struct FAtmosphereProperties : public FTableRowBase
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
	float Intensity = 3;
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "0"))
	float MieHeightScale = 0.25f;
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "0"))
	float OpacityMultiplier = 1;

	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "1"))
	int8 Steps_i = 16;
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "1"))
	int8 Steps_j = 4;

	UPROPERTY(SaveGame, EditAnywhere)
	FLinearColor Ambient = FVector4(0, 0, 0, 0);
	UPROPERTY(SaveGame, EditAnywhere)
	FLinearColor Mie = FVector4(.5f, .5f, .5f, .5f);
	UPROPERTY(SaveGame, EditAnywhere)
	FLinearColor Ray = FVector4(.333f, .666f, 1, 1);
};
