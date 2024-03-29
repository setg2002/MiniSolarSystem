// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CloudComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINISOLARSYSTEM_API UCloudComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UCloudComponent();

	void CreateMaterial();

	UMaterialInstanceDynamic* DynamicMaterial;

	/*UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float CloudHeight = 1.1;*/

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual void OnComponentCreated() override;

	virtual void BeginPlay() override;

};
