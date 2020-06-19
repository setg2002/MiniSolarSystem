// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EFilterType.h"
#include "NoiseSettings.generated.h"

/**
 * 
 */
/*UENUM()
enum EFilterType
{
	Simple  UMETA(DisplayName = "Simple"),
	Ridgid  UMETA(DisplayName = "Ridgid")
};*/

USTRUCT(BlueprintType)
struct FSimpleNoiseSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float Strength = 1;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float BaseRoughness = 1;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float Roughness = 2;
	UPROPERTY(EditAnywhere)
	FVector Center;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "8"))
	int numLayers = 1;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "2"))
	float Persistence = .5f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MinValue;
};

USTRUCT(BlueprintType)
struct FRidgidNoiseSettings : public FSimpleNoiseSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float WeightMultiplier = .8f;
};

UCLASS(EditInlineNew)
class CPPGAME_API UNoiseSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UNoiseSettings();
	~UNoiseSettings();

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EFilterType> FilterType;

	UPROPERTY(EditAnywhere)
	FSimpleNoiseSettings SimpleNoiseSettings;
	UPROPERTY(EditAnywhere)
	FRidgidNoiseSettings RidgidNoiseSettings;
};
