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
		FColor(255, 178, 178),
		FColor(207, 238, 255),
		FColor(255, 233, 208),
		FColor(255, 187, 173),
		FColor(232, 254, 255),
		FColor(250, 239, 255)
	};

};
