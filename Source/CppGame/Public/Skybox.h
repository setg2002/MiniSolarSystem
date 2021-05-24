// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skybox.generated.h"


UCLASS()
class CPPGAME_API ASkybox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkybox();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, CallInEditor)
	void MakeTexture();
#endif
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D StarOffset = FVector2D(0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D NoiseOffset = FVector2D(.5f, .1f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D ColorOffset = FVector2D(-.75f, .5f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	float NoiseWarpAmount = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float ColorMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1"))
	float DesaturationAmount = .45f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float Intensity = 2.f;

	virtual void OnConstruction(const FTransform & Transform) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UStaticMeshComponent* Mesh;

private:
	UMaterialInstanceDynamic* DynamicMaterial;

	const TArray<FColor> possibleColors = {
		FColor(255, 210, 210),
		FColor(235, 247, 255),
		FColor(255, 242, 224),
		FColor(255, 237, 229),
		FColor(232, 254, 255),
		FColor(250, 239, 255)
	};

};
