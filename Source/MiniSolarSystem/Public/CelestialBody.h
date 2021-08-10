// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "CelestialObject.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CelestialBody.generated.h"


//Forward Declarations
class ACelestialGameMode;
class UWidgetComponent;

UCLASS(Blueprintable)
class MINISOLARSYSTEM_API ACelestialBody : public AActor, public ICelestialObject
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
	// Returns true of the name of the body was set to NewName
	bool SetName(FName NewName);

	// Sets the magnitude of the current velocity to NewVelocity without changing direction
	UFUNCTION(BlueprintCallable)
	void ScaleCurrentVelocity(float NewVelocity);
	UFUNCTION(BlueprintCallable)
	void SetCurrentVelocity(FVector NewVelocity);

	UFUNCTION(BlueprintCallable)
	void AddCelestialComponent(UStaticMeshComponent* NewComp);

	USphereComponent* Collider;

	// Called when collider is overlapped with
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called when collider overlap ends
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(SaveGame, EditInstanceOnly, Category = "Default")
	int mass;

	UPROPERTY(SaveGame)
	FVector currentVelocity;

	ACelestialGameMode* gameMode;

	// Returns an FName with _n if the InName is already taken
	FName EnsureUniqueName(FName InName);

private:
	// The actor that is currently being overlapped with, nullptr if not overlapping
	AActor* OverlappedActor;

public:
	// The CelestialBody that orbitVelocity is to be calculated for
	UPROPERTY(SaveGame, Category = "Orbits", EditAnywhere, BlueprintReadWrite)
	ACelestialBody* OrbitingBody;

	// The necessary velocity for this planet to orbit OrbitingBody
	UPROPERTY(SaveGame, Category = "Orbits", VisibleAnywhere, BlueprintReadOnly)
	float orbitVelocity;

	// Sets orbitVelocity to the required magnitude to orbit OrbitingBody 
	UFUNCTION(Category = "Orbits", BlueprintCallable, CallInEditor)
	void CalculateOrbitVelocity();

	// Sets velocity to orbitVelocity
	UFUNCTION(Category = "Orbits", BlueprintCallable, CallInEditor)
	void SetToOrbit();

	// Shows/hides orbit calculation vectors
	UPROPERTY(Category = "Orbits", AdvancedDisplay, EditAnywhere, BlueprintReadWrite)
	bool bVectorDebug;

	UPROPERTY(Category = "Orbits", AdvancedDisplay, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bVectorDebug"))
	int32 VectorLength = 50;

	UPROPERTY(Category = "Orbits", AdvancedDisplay, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bVectorDebug"))
	float VectorSize = 1;

	UPROPERTY(Category = "Orbits", AdvancedDisplay, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bVectorDebug"))
	int VectorThickness = 1;

	UPROPERTY(Category = "Orbits", AdvancedDisplay, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bVectorDebug"))
	int32 VectorDuration = 10;
};
