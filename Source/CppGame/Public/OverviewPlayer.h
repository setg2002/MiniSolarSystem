// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OverviewPlayer.generated.h"

class ACelestialGameMode;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void SwitchPerspective();

	void MoveRight(float AxisValue)
	{
		RootComponent->AddLocalRotation(FQuat(0, 0, -AxisValue, 0));
	}


	ACelestialGameMode* gameMode;
};
