// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RingSystemComponent.generated.h"

class UStaticMeshComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPGAME_API URingSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URingSystemComponent();

	UStaticMeshComponent* RingMesh;

	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.001"))
	float OuterRadius = 1;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.01", ClampMax = "1"))
	float InnerRadius;

	UPROPERTY(EditAnywhere)
	UCurveLinearColor* Gradient;

	UTexture2D* GradientTexture;
	
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	virtual void OnComponentCreated() override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
