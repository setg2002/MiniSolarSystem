// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "CelestialObject.h"
#include "GameFramework/Pawn.h"
#include "CelestialPlayer.generated.h"


class ACelestialGameMode;
class UCameraComponent;

UCLASS()
class CPPGAME_API ACelestialPlayer : public APawn, public ICelestialObject
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACelestialPlayer();

	UPROPERTY(BlueprintReadWrite)
	bool bFocusPlanet = false;

	UFUNCTION(BlueprintCallable)
	virtual int GetMass() const override;

	UFUNCTION()
	virtual void UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep) override;

	UFUNCTION()
	virtual void UpdatePosition(float timeStep) override;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetCurrentVelocity() const override;

	// Returns the throttle value
	UFUNCTION(BlueprintCallable)
	float GetThrottle()
	{
		return Throttle;
	}

	UFUNCTION(BlueprintCallable)
	bool GetIgnoreGravity()
	{
		return bIgnoreGravity;
	}

	UFUNCTION(BlueprintCallable)
	ACelestialBody* GetLargestForce();

	UFUNCTION(BlueprintCallable)
	ACelestialBody* LookingAtPlanet();

	void SetWidget(UUserWidget* Highlight) { Widget = Highlight; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	bool bIgnoreGravity = false;

	const int mass = 10;

	const int MaxSpeed = 9999;

	UUserWidget* Widget;

	//UCameraComponent* Camera; //NOTE: Making components into UPROPERTY() makes them NULL at runtime

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PlanetHighlightClass;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveUp(float AxisValue);

	FVector IntendedRotation;
	void RotationX(float AxisValue);
	void RotationY(float AxisValue);
	void RotationZ(float AxisValue);

	void SwitchPerspective();

	void SwitchIgnoreGravity()
	{
		bIgnoreGravity = !bIgnoreGravity;
	}

	void SwitchFocusPlanet()
	{
		bFocusPlanet = !bFocusPlanet;
	}
	
	UPROPERTY(EditAnywhere)
	int RotationSpeed = 10;
	UPROPERTY(EditAnywhere)
	int ThrustForce = 10;
	float Throttle = 1;

	void ChangeThrottle(float AxisValue)
	{
		if (Throttle + AxisValue / 10.f > 0.09f && Throttle + AxisValue / 10.f <= 5)
		{
			Throttle += AxisValue / 10.f;
		}
	}

	void LimitVelocity();

	ACelestialGameMode* gameMode;

	TMap<ACelestialBody*, float> ForcePerBody;
};
