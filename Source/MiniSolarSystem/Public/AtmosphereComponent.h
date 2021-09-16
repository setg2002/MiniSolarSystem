// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "AtmosphereProperties.h"
#include "AtmosphereComponent.generated.h"

/**
 * 
 */


UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINISOLARSYSTEM_API UAtmosphereComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(SaveGame, EditAnywhere)
	FAtmosphereProperties AtmosphereProperties;

	UPROPERTY(SaveGame, EditAnywhere)
	bool bClouds = true;

	UPROPERTY(SaveGame, EditAnywhere, AdvancedDisplay)
	float CloudHeight;

public:
	UAtmosphereComponent();

	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
	class UCloudComponent* CloudComponent;

	float PlanetRadius;

	UFUNCTION(BlueprintCallable)
	FAtmosphereProperties GetProperties() const { return AtmosphereProperties; }
	UFUNCTION(BlueprintCallable)
	void SetProperties(FAtmosphereProperties PropertiesToSet);

	UFUNCTION(BlueprintCallable)
	bool GetClouds() const { return bClouds; }
	UFUNCTION(BlueprintCallable)
	void SetClouds(bool NewClouds);

	UFUNCTION(BlueprintCallable)
	float GetCloudHeight() const { return CloudHeight; }
	UFUNCTION(BlueprintCallable)
	void SetCloudHeight(float NewCloudHeight);

	UFUNCTION(BlueprintCallable)
	float GetHeight() const { return AtmosphereProperties.Height; }
	UFUNCTION(BlueprintCallable)
	void SetHeight(float NewHeight);

	UFUNCTION(BlueprintCallable)
	float GetDensity() const { return AtmosphereProperties.Density; }
	UFUNCTION(BlueprintCallable)
	void SetDensity(float NewDensity);

	UFUNCTION(BlueprintCallable)
	float GetG() const { return AtmosphereProperties.g; }
	UFUNCTION(BlueprintCallable)
	void SetG(float NewG);

	UFUNCTION(BlueprintCallable)
	float GetMieHeightScale() const { return AtmosphereProperties.MieHeightScale; }
	UFUNCTION(BlueprintCallable)
	void SetMieHeightScale(float NewMieHeightScale);

	UFUNCTION(BlueprintCallable)
	float GetOpacityMultiplier() const { return AtmosphereProperties.OpacityMultiplier; }
	UFUNCTION(BlueprintCallable)
	void SetOpacityMultiplier(float NewOpacityMultiplier);

	UFUNCTION(BlueprintCallable)
	int GetStepsI() const { return AtmosphereProperties.Steps_i; }
	UFUNCTION(BlueprintCallable)
	int GetStepsJ() const { return AtmosphereProperties.Steps_j; }

	UFUNCTION(BlueprintCallable)
	FLinearColor GetAmbient() const { return AtmosphereProperties.Ambient; }
	UFUNCTION(BlueprintCallable)
	void SetAmbient(FLinearColor NewAmbient);

	UFUNCTION(BlueprintCallable)
	FLinearColor GetMie() const { return AtmosphereProperties.Mie; }
	UFUNCTION(BlueprintCallable)
	void SetMie(FLinearColor NewMie);

	UFUNCTION(BlueprintCallable)
	FLinearColor GetRay() const { return AtmosphereProperties.Ray; }
	UFUNCTION(BlueprintCallable)
	void SetRay(FLinearColor NewRay);

	void UpdateProperties();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void DestroyComponent(bool bPromoteChildren) override;

protected:
	virtual void OnComponentCreated() override;

	virtual void BeginPlay() override;

};
