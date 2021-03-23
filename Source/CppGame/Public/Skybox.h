// This is a copyright notice

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

	UFUNCTION(BlueprintCallable, CallInEditor)
	void/*UTexture2D*/ MakeTexture();

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
