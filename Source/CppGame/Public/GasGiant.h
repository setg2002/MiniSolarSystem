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
class USphereComponent;


UCLASS()
class CPPGAME_API AGasGiant : public ACelestialBody
{
	GENERATED_BODY()
	
public:
	AGasGiant();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	UGasGiantColorSettings* ColorSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* Collider;

	UFUNCTION(BlueprintCallable)
	int GetRadius() const { return Radius; }
	UFUNCTION(BlueprintCallable)
	void SetRadius(int NewRadius);

	GaseousColorGenerator* ColorGenerator;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "1"))
	int Radius = 1;

	virtual void BeginPlay() override;
};
