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
class MINISOLARSYSTEM_API AGasGiant : public ACelestialBody
{
	GENERATED_BODY()
	
public:
	AGasGiant();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	UGasGiantColorSettings* ColorSettings;

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
};
