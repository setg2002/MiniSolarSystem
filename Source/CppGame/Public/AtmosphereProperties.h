// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine\DataTable.h"
#include "AtmosphereProperties.generated.h"


USTRUCT(BlueprintType)
struct FAtmosphereProperties : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float Height = 0;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float Density = 0.15f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float g = 0.8f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float Intensity = 3;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MieHeightScale = 0.25f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float OpacityMultiplier = 1;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1"))
	int8 Steps_i = 16;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1"))
	int8 Steps_j = 4;

	UPROPERTY(EditAnywhere)
	FLinearColor Ambient = FVector4(0, 0, 0, 0);
	UPROPERTY(EditAnywhere)
	FLinearColor Mie = FVector4(.5f, .5f, .5f, .5f);
	UPROPERTY(EditAnywhere)
	FLinearColor Ray = FVector4(.333f, .666f, 1, 1);
};
