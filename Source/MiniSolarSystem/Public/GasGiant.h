// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "GasGiantColorSettings.h"
#include "GasGiant.generated.h"

/**
 * 
 */

// Forward Declarations
class GaseousColorGenerator;


UCLASS()
class MINISOLARSYSTEM_API AGasGiant : public ACelestialBody
{
	GENERATED_BODY()
	
public:
	AGasGiant();

	// Used to reinitialize the gas giant after loading
	void ReInit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	FGasGiantColorSettings ColorSettings;

	UFUNCTION(BlueprintCallable)
	void SetRadius(float NewRadius);

	virtual float GetBodyRadius() const override { return Radius * 100; }

	GaseousColorGenerator* ColorGenerator;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "1"))
	float Radius = 1;

	virtual void BeginPlay() override;

public:
	UMaterialInterface* BasePlanetMat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInstanceDynamic* DynamicMaterial;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateMaterial();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void NewVoronoiForStorms();

	UFUNCTION(BlueprintCallable)
	UCurveLinearColor* GetGradient() const { return ColorSettings.Gradient; }
	UFUNCTION(BlueprintCallable)
	void SetGradient(UCurveLinearColor* NewGradient);

	// ~~~ Voronoi Settings ~~~

	UFUNCTION(BlueprintCallable)
	int GetNumStorms() const { return ColorSettings.NumStorms; }
	UFUNCTION(BlueprintCallable)
	void SetNumStorms(int NewNumStorms);
	
	UFUNCTION(BlueprintCallable)
	float GetStormFalloff() const { return ColorSettings.StormFalloff; }
	UFUNCTION(BlueprintCallable)
	void SetStormFalloff(float NewStormFalloff);

	const int LowBound = 100;
	const int HighBound = 924;
};
