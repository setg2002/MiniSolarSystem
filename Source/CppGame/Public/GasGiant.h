// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "GasGiant.generated.h"

/**
 * 
 */

// Forward Declarations
class UGasGiantColorSettings;
class GaseousColorGenerator;


UCLASS()
class CPPGAME_API AGasGiant : public ACelestialBody
{
	GENERATED_BODY()
	
public:
	AGasGiant();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGasGiantColorSettings* ColorSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int Radius = 1;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateMaterial();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void NewVoronoiForStorms();

	GaseousColorGenerator* ColorGenerator;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
