// Copyright Soren Gilbertson


#include "CelestialPlayer.h"
#include "CelestialBody.h"
#include "CelestialGameMode.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACelestialPlayer::ACelestialPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACelestialPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	gameMode = Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void ACelestialPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller)
	{
		UpdatePosition(DeltaTime);

		// Camera Rotation Lag
		Controller->SetControlRotation(FMath::Lerp<FRotator>(Controller->GetControlRotation(), IntendedRotation, DeltaTime * RotationSpeed));
	}
}


// Called to bind functionality to input
void ACelestialPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACelestialPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACelestialPlayer::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ACelestialPlayer::MoveUp);

	PlayerInputComponent->BindAxis("RotationX", this, &ACelestialPlayer::RotationX);
	PlayerInputComponent->BindAxis("RotationY", this, &ACelestialPlayer::RotationY);

	PlayerInputComponent->BindAxis("ChangeSpeed", this, &ACelestialPlayer::ChangeThrottle);

	PlayerInputComponent->BindAction("SwitchPerspective", EInputEvent::IE_Released, this, &ACelestialPlayer::SwitchPerspective);
	PlayerInputComponent->BindAction("IgnoreGravity", EInputEvent::IE_Released, this, &ACelestialPlayer::SwitchIgnoreGravity);
}


void ACelestialPlayer::UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep)
{
	if (!bIgnoreGravity)
	{
		for (int i = 0; i < allBodies.Num(); i++)
		{
			ACelestialBody* otherBody = allBodies[i];

			float sqrDst = (otherBody->GetActorLocation() - this->GetActorLocation()).Size();
			FVector forceDir = (otherBody->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
			FVector force = forceDir * gameMode->gravitationalConstant * mass * otherBody->mass / sqrDst;
			FVector acceleration = force / mass;
			this->currentVelocity += acceleration * timeStep;
		}
	}
}

void ACelestialPlayer::UpdatePosition(float timeStep)
{
	this->SetActorLocation(FVector(this->GetActorLocation() + (currentVelocity * timeStep)));
}


void ACelestialPlayer::MoveForward(float AxisValue)
{
	if (Controller)
	{
		currentVelocity += (Controller->GetControlRotation().Vector() * AxisValue * Throttle);
	}
}

void ACelestialPlayer::MoveRight(float AxisValue)
{
	if (Controller)
	{
		currentVelocity += (UKismetMathLibrary::GetRightVector(Controller->GetControlRotation()) * AxisValue * Throttle);
	}
}

void ACelestialPlayer::MoveUp(float AxisValue)
{
	if (Controller)
	{
		currentVelocity += (UKismetMathLibrary::GetUpVector(Controller->GetControlRotation()) * AxisValue * Throttle);
	}
}


void ACelestialPlayer::SwitchPerspective()
{
	gameMode->SetPerspective(0);
}
