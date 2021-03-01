// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "RingSystemComponent.generated.h"



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPGAME_API URingSystemComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URingSystemComponent();

	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.001", ClampMax = "2"))
	float Radius = 1;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.01", ClampMax = "1"))
	float RingWidth = 0.01f;

	UPROPERTY(EditAnywhere)
	UCurveLinearColor* Gradient;

	UTexture2D* GradientTexture;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnComponentCreated() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
