// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "CelestialObject.h"
#include "GameFramework/Actor.h"
#include "CelestialBody.generated.h"


//Forward Declarations
class ACelestialGameMode;

UCLASS(Blueprintable)
class CPPGAME_API ACelestialBody : public AActor, public ICelestialObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACelestialBody();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = "Default")
	int mass;

	UPROPERTY(EditInstanceOnly, Category = "Default")
	FVector initialVelocity;

	UPROPERTY(EditInstanceOnly, Category = "Default")
	float rotationRate;

	UFUNCTION(BlueprintCallable)
	virtual int GetMass() const override;

	UFUNCTION()
	virtual void UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep) override;

	UFUNCTION()
	virtual void UpdatePosition(float timeStep) override;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetCurrentVelocity() const override;

	ACelestialGameMode* gameMode;

	USceneComponent* Root;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
