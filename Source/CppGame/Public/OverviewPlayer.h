// This is a copyright notice

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* Root;

	USpringArmComponent* SpringArm;

	UCameraComponent* Camera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void SwitchPerspective();

	void RotateX(float AxisValue)
	{
		RootComponent->AddLocalRotation(FRotator(0, AxisValue, 0));
	}
	void RotateY(float AxisValue);

	void Zoom(float AxisValue);

	ACelestialGameMode* gameMode;
};
