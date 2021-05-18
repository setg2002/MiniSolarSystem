// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GasGiantColorSettings.generated.h"

/**
 * 
 */


UCLASS()
class CPPGAME_API UGasGiantColorSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UGasGiantColorSettings();

	void SetMesh(UStaticMeshComponent* mesh) { Mesh = mesh; }

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BasePlanetMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveLinearColor* Gradient;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInstanceDynamic* DynamicMaterial;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateMaterial();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void NewVoronoiForStorms();

	// Voronoi Settings \\

	UFUNCTION(BlueprintCallable)
	int GetNumStorms() const { return NumStorms; }
	UFUNCTION(BlueprintCallable)
	void SetNumStorms(int NewNumStorms);
	
	UFUNCTION(BlueprintCallable)
	float GetStormFalloff() const { return StormFalloff; }
	UFUNCTION(BlueprintCallable)
	void SetStormFalloff(float NewStormFalloff);

	//UPROPERTY(EditAnywhere, Category = "Storm Settings", AdvancedDisplay, meta = (ClampMin = "0", ClampMax = "1024"))
	const int LowBound = 100;

	//UPROPERTY(EditAnywhere, Category = "Storm Settings", AdvancedDisplay, meta = (ClampMin = "0", ClampMax = "1024"))
	const int HighBound = 924;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Storm Settings", meta = (ClampMin = "0"))
	int NumStorms = 25;

	UPROPERTY(EditAnywhere, Category = "Storm Settings", meta = (ClampMin = "0"))
	float StormFalloff = 2.5f;
};
