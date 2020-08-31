// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "Star.generated.h"

/**
 * 
 */

UENUM()
enum EStarType {
	O   UMETA(DisplayName = "Blue Supergiant"),
	B	UMETA(DisplayName = "Blue Giant"),
	A	UMETA(DisplayName = "White Giant"),
	F	UMETA(DisplayName = "Yellow Giant"),
	G	UMETA(DisplayName = "Sun"),
	K	UMETA(DisplayName = "Orange Dwarf"),
	M	UMETA(DisplayName = "Brown Dwarf"),
	D	UMETA(DisplayName = "White Dwarf")
};

USTRUCT(BlueprintType)
struct FStarProperties : public FTableRowBase 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1"))
	int radius = 50;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1"))
	int mass = 1000;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	int luminosity = 50;

	UPROPERTY(EditAnywhere)
	FColor color = FColor(128, 0, 0);
};

UCLASS(hidecategories = ("Default"))
class CPPGAME_API AStar : public ACelestialBody
{
	GENERATED_BODY()

public:
	AStar();

	UPROPERTY(EditInstanceOnly)
	TEnumAsByte<EStarType> starType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* sphere;

	UPROPERTY(EditAnywhere)
	FStarProperties starProperties;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* starTypeData;

	UMaterialInstanceDynamic* dynamicMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialParameterCollection* planetMateralParameterCollection;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	UMaterialParameterCollectionInstance* planetMateralParameterCollectionInst;
};
