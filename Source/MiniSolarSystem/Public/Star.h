// Copyright Soren Gilbertson

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
class MINISOLARSYSTEM_API AStar : public ACelestialBody
{
	GENERATED_BODY()

public:
	AStar();

	UPROPERTY(EditInstanceOnly)
	TEnumAsByte<EStarType> starType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FStarProperties starProperties;
	bool SetStarProperties(FStarProperties NewProperties);

	UPROPERTY(EditDefaultsOnly)
	UDataTable* starTypeData;

	UMaterialInstanceDynamic* dynamicMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialParameterCollection* planetMateralParameterCollection;

	UFUNCTION(CallInEditor, BlueprintCallable, Category="Particle Debug")
	void ReInitParticles();

	UFUNCTION(BlueprintCallable)
	void SetRadius(float NewRadius);

	UFUNCTION(BlueprintCallable)
	int32 GetLuminosity() const { return starProperties.luminosity; }
	UFUNCTION(BlueprintCallable)
	void SetLuminosity(int NewLuminosity);

	UFUNCTION(BlueprintCallable)
	void SetColor(FColor NewColor);

	void UpdateColor();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float GetBodyRadius() const override { return starProperties.radius * 100; }

	void SetStarNum(uint8 num);

	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* GetParticleComp() const { return ParticleComponent; }

protected:
	UMaterialParameterCollectionInstance* PlanetIlluminationInst;

	UNiagaraComponent* ParticleComponent;

	UDirectionalLightComponent* Light;

	UNiagaraSystem* SolarParticleTemplate;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostEditMove(bool bFinished) override;
#endif

	virtual void OnConstruction(const FTransform & Transform) override;

private:
	FStarProperties OldProperties;

	// The number of this star used for planetMateralParameterCollectionInst
	uint8 StarNum = 5;

};
