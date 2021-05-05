// This is a copyright notice

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDampInertia = false;

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

	void RotationX(float AxisValue)
	{
		AddControllerYawInput(AxisValue);
	}
	void RotationY(float AxisValue)
	{
		AddControllerPitchInput(AxisValue);
	}

	void SwitchPerspective();

	ACelestialGameMode* gameMode;
};
