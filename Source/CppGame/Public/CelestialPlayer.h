// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "CelestialObject.h"
#include "GameFramework/Pawn.h"
#include "CelestialPlayer.generated.h"


class ACelestialGameMode;

UCLASS()
class CPPGAME_API ACelestialPlayer : public APawn, public ICelestialObject
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACelestialPlayer();

	UFUNCTION()
	virtual void UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep) override;

	UFUNCTION()
	virtual void UpdatePosition(float timeStep) override;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	bool bIgnoreGravity = false;

	const int mass = 10;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveUp(float AxisValue);

	FRotator IntendedRotation;
	void RotationX(float AxisValue)
	{
		if (Controller)
		{
			IntendedRotation += FRotator(0, AxisValue, 0);
		}
	}
	void RotationY(float AxisValue)
	{
		if (Controller)
		{
			IntendedRotation += FRotator(-AxisValue, 0, 0);
		}
	}

	void SwitchPerspective();

	void SwitchIgnoreGravity()
	{
		bIgnoreGravity = !bIgnoreGravity;
	}

	int RotationSpeed = 10;
	float Throttle = 1;

	void ChangeThrottle(float AxisValue)
	{
		if (Throttle + AxisValue / 10.f > 0.09f && Throttle + AxisValue / 10.f <= 5)
		{
			Throttle += AxisValue / 10.f;
		}
	}

	ACelestialGameMode* gameMode;
};
