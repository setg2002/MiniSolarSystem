// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OverviewPlayer.generated.h"

class USpringArmComponent;
class ACelestialGameMode;
class UCameraComponent;

UCLASS()
class CPPGAME_API AOverviewPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOverviewPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1", ClampMax = "2000"))
	int Speed = 150;

	FVector GetCameraLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void SwitchPerspective();

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveUp(float AxisValue);

	void RotateX(float AxisValue)
	{
		this->AddActorWorldRotation(FRotator(0, AxisValue, 0));
	}
	void RotateY(float AxisValue);

	void Zoom(float AxisValue);

	void ChangeSpeed(float AxisValue)
	{
		if (Speed + AxisValue * 50 > 1 && Speed + AxisValue * 50 <= 2000)
		{
			Speed += AxisValue * 50;
		}
	}

	ACelestialGameMode* gameMode;
};
