// Copyright Soren Gilbertson


#include "OverviewPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "DataAssets/InputDataConfig.h"
#include "BodySystemFunctionLibrary.h"
#include "Game/CelestialGameState.h"
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

void AOverviewPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOverviewPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller)
	{
		// Mouse drag rotation w/ left click
		if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(FKey(EKeys::LeftMouseButton)) && bCanRot)
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

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
 
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	
	ACelestialGameState* GameState = GetWorld()->GetGameState<ACelestialGameState>();
	
	if (!GameState) return;
	
	GameState->SetOverviewMappingContext(InputMappingContext);
	
	Subsystem->RemoveMappingContext(GameState->GetCelestialInputMappingContext());
	Subsystem->AddMappingContext(InputMappingContext, 0);
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    
	if(!OverviewInputConfig) return; // Don't wanna bind to invalid UInputActions!
    
	Input->BindAction(OverviewInputConfig->Move, ETriggerEvent::Triggered, this, &AOverviewPlayer::Move);
	Input->BindAction(OverviewInputConfig->Look, ETriggerEvent::Triggered, this, &AOverviewPlayer::Rotate);
	Input->BindAction(OverviewInputConfig->ChangeSpeed, ETriggerEvent::Triggered, this, &AOverviewPlayer::ChangeSpeed);
	Input->BindAction(OverviewInputConfig->Zoom, ETriggerEvent::Triggered, this, &AOverviewPlayer::Zoom);
}

void AOverviewPlayer::Move(const FInputActionValue& Value)
{
	SetActorLocation(GetActorLocation() + (GetActorRotation().RotateVector(Value.Get<FVector>()) * Speed));
}

void AOverviewPlayer::Rotate(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	SpringArm->AddLocalRotation(FRotator(InputVector.Y, 0, 0));
	this->AddActorWorldRotation(FRotator(0, InputVector.X, 0));
}

void AOverviewPlayer::ChangeSpeed(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	if (Speed + InputValue * 50 > 1 && Speed + InputValue * 50 <= 2000)
	{
		Speed += InputValue * 50;
	}
}

void AOverviewPlayer::Zoom(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	if (SpringArm->TargetArmLength + (InputValue * Speed) >= 0)
	{
		SpringArm->TargetArmLength += (InputValue * Speed);
	}
}

FVector AOverviewPlayer::GetCameraLocation()
{
	return Camera->GetComponentLocation();
}
