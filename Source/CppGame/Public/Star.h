// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "StarProperties.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStarProperties starProperties;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* starTypeData;

	UMaterialInstanceDynamic* dynamicMaterial;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UMaterialInstance* distStarMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialParameterCollection* planetMateralParameterCollection;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/*UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateDistantStars();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> distantStarBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int numDistantStars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int minSpawnRange = 50000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int maxSpawnRange = 200000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FColor> possibleColors;

	TArray<UMaterialInstanceDynamic*> dynamMats;*/

	void UpdateColor();

protected:
	UMaterialParameterCollectionInstance* planetMateralParameterCollectionInst;

	//TArray<AActor*> distantStars;
};
