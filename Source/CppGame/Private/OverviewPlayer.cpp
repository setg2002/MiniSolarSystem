// Copyright Soren Gilbertson


#include "OverviewPlayer.h"
#include "CelestialGameMode.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AOverviewPlayer::AOverviewPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 50000;
	SpringArm->bDoCollisionTest = 0;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AOverviewPlayer::BeginPlay()
{
	Super::BeginPlay();

	gameMode = Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AOverviewPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller)
	{
		// Mouse drag rotation w/ left click
		if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(FKey(EKeys::LeftMouseButton)))
		{
			float speedMultiplier = 3.f;
			float dX;
			float dY;
			GetWorld()->GetFirstPlayerController()->GetInputMouseDelta(dX, dY);

			SpringArm->AddLocalRotation(FRotator(speedMultiplier * dY, 0, 0));
			this->AddActorWorldRotation(FRotator(0, speedMultiplier * dX, 0));
		}

		// Mouse drag pan w/ middle click
		if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(FKey(EKeys::MiddleMouseButton)) && bCanPan)
		{
			float speedMultiplier = 100.f;
			float dX;
			float dY;
			GetWorld()->GetFirstPlayerController()->GetInputMouseDelta(dX, dY);

			this->SetActorLocation(this->GetActorLocation() + (speedMultiplier * dX * Camera->GetRightVector()) + (speedMultiplier * dY * Camera->GetUpVector()));
		}

		// Mouse drag movememt w/ right click
		if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(FKey(EKeys::RightMouseButton)))
		{
			float speedMultiplier = 100.f;
			float dX;
			float dY;
			GetWorld()->GetFirstPlayerController()->GetInputMouseDelta(dX, dY);

			this->SetActorLocation(this->GetActorLocation() + (Root->GetForwardVector() * (dY * Speed)) + (Root->GetRightVector() * (dX * Speed)));
		}
	}
}

// Called to bind functionality to input
void AOverviewPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("SwitchPerspective", EInputEvent::IE_Released, this, &AOverviewPlayer::SwitchPerspective);

	PlayerInputComponent->BindAxis("Overview_MoveForward", this, &AOverviewPlayer::MoveForward);
	PlayerInputComponent->BindAxis("Overview_MoveRight", this, &AOverviewPlayer::MoveRight);
	PlayerInputComponent->BindAxis("Overview_MoveUp", this, &AOverviewPlayer::MoveUp);

	PlayerInputComponent->BindAxis("Overview_RotationX", this, &AOverviewPlayer::RotateX);
	PlayerInputComponent->BindAxis("Overview_RotationY", this, &AOverviewPlayer::RotateY);
	PlayerInputComponent->BindAxis("Zoom", this, &AOverviewPlayer::Zoom);
	
	PlayerInputComponent->BindAxis("Overview_IncreaseSpeed", this, &AOverviewPlayer::ChangeSpeed);

}

void AOverviewPlayer::SwitchPerspective()
{
	gameMode->SetPerspective(1);
}

void AOverviewPlayer::RotateY(float AxisValue)
{
	SpringArm->AddLocalRotation(FRotator(AxisValue, 0, 0));
}

void AOverviewPlayer::Zoom(float AxisValue)
{
	if (SpringArm->TargetArmLength + (AxisValue * Speed) >= 0)
	{
		SpringArm->TargetArmLength += (AxisValue * Speed);
	}
}

void AOverviewPlayer::MoveForward(float AxisValue)
{
	this->SetActorLocation(this->GetActorLocation() + (Root->GetForwardVector() * (AxisValue * Speed)));
}

void AOverviewPlayer::MoveRight(float AxisValue)
{
	this->SetActorLocation(this->GetActorLocation() + (Root->GetRightVector() * (AxisValue * Speed)));
}

void AOverviewPlayer::MoveUp(float AxisValue)
{
	this->AddActorLocalOffset(FVector::UpVector * (AxisValue * Speed));
}

FVector AOverviewPlayer::GetCameraLocation()
{
	return Camera->GetComponentLocation();
}
