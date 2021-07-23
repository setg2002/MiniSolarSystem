// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "CelestialObject.h"
#include "GameFramework/Actor.h"
#include "CelestialBody.generated.h"


//Forward Declarations
class ACelestialGameMode;
class UWidgetComponent;

UCLASS(Blueprintable)
class CPPGAME_API ACelestialBody : public AActor, public ICelestialObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(SaveGame, EditAnywhere)
	FName BodyName = NAME_None;

public:	
	// Sets default values for this actor's properties
	ACelestialBody();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(SaveGame, EditInstanceOnly, Category = "Default")
	FVector initialVelocity;

	UPROPERTY(SaveGame, EditInstanceOnly, BlueprintReadWrite, Category = "Default")
	float rotationRate;

	UFUNCTION(BlueprintCallable)
	void RequestDestroyComponent(UActorComponent* ComponentToDestroy);

	UFUNCTION(BlueprintCallable)
	int SetMass(int newMass);

	UFUNCTION(BlueprintCallable)
	virtual int GetMass() const override;

	UFUNCTION()
	virtual void UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep) override;

	UFUNCTION()
	virtual void UpdatePosition(float timeStep) override;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetCurrentVelocity() const override;

	// Sets the location of the body and re-draws orbits
	UFUNCTION(BlueprintCallable)
	void SetLocation(FVector NewLocation);

	UFUNCTION(BlueprintCallable)
	FName GetBodyName() const { return BodyName; }
	UFUNCTION(BlueprintCallable)
	bool SetName(FName NewName);

	// Sets the magnitude of the current velocity to NewVelocity without changing direction
	UFUNCTION(BlueprintCallable)
	void ScaleCurrentVelocity(float NewVelocity);
	UFUNCTION(BlueprintCallable)
	void SetCurrentVelocity(FVector NewVelocity);

	UFUNCTION(BlueprintCallable)
	void AddCelestialComponent(UStaticMeshComponent* NewComp);

	USceneComponent* Root;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(SaveGame, EditInstanceOnly, Category = "Default")
	int mass;

	UPROPERTY(SaveGame)
	FVector currentVelocity;

	ACelestialGameMode* gameMode;
};
