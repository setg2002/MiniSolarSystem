// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "AtmosphereProperties.h"
#include "AtmosphereComponent.generated.h"

/**
 * 
 */

class UCloudComponent;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CPPGAME_API UAtmosphereComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UAtmosphereComponent(/*float radius*/);

	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
	UCloudComponent* CloudComponent;

	UPROPERTY(EditAnywhere)
	FAtmosphereProperties AtmosphereProperties;

	float PlanetRadius;

	UPROPERTY(EditAnywhere)
	bool bClouds = true;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	float CloudHeight;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void DestroyComponent(bool bPromoteChildren) override;

protected:
	virtual void OnComponentCreated() override;

};
