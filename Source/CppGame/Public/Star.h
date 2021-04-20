// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "StarProperties.h"
#include "Star.generated.h"


/**
 * 
 */

class UNiagaraSystem;
class UNiagaraComponent;
class UDirectionalLightComponent;

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
	UStaticMeshComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStarProperties starProperties;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* starTypeData;

	UMaterialInstanceDynamic* dynamicMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialParameterCollection* planetMateralParameterCollection;

	UFUNCTION(CallInEditor, BlueprintCallable, Category="Particle Debug")
	void ReInitParticles();

	void UpdateColor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UMaterialParameterCollectionInstance* planetMateralParameterCollectionInst;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* ParticleComponent;

	UDirectionalLightComponent* Light;

	UNiagaraSystem* SolarParticleTemplate;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostEditMove(bool bFinished) override;
#endif

	virtual void OnConstruction(const FTransform & Transform) override;

};
