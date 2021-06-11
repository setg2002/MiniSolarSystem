// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "RingSystemComponent.generated.h"


class GaseousColorGenerator;


UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPPGAME_API URingSystemComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URingSystemComponent();

private:
	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "0.001", ClampMax = "2"))
	float Radius = 1;

	UPROPERTY(SaveGame, EditAnywhere, meta = (ClampMin = "0.01", ClampMax = "1"))
	float RingWidth = 0.01f;

	UPROPERTY(SaveGame, EditAnywhere)
	UCurveLinearColor* Gradient;

	UMaterialInstanceDynamic* DynamicMaterial;

public:
	UFUNCTION(BlueprintCallable)
	float GetRadius() const { return Radius; }
	UFUNCTION(BlueprintCallable)
	void SetRadius(float NewRadius);

	UFUNCTION(BlueprintCallable)
	float GetWidth() const { return RingWidth; }
	UFUNCTION(BlueprintCallable)
	void SetWidth(float NewWidth);

	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetDynamicMaterial() const { return DynamicMaterial; }

	void UpdateProperties();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnComponentCreated() override;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void CreateMaterial();
	
	GaseousColorGenerator* ColorGenerator;

	UTexture2D* GradientTexture;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
