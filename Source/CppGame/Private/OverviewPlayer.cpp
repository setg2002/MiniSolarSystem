// This is a copyright notice


#include "OverviewPlayer.h"
#include "CelestialGameMode.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


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

}

// Called to bind functionality to input
void AOverviewPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("SwitchPerspective", EInputEvent::IE_Released, this, &AOverviewPlayer::SwitchPerspective);

	PlayerInputComponent->BindAxis("Overview_RotationX", this, &AOverviewPlayer::RotateX);
	PlayerInputComponent->BindAxis("Overview_RotationY", this, &AOverviewPlayer::RotateY);
	PlayerInputComponent->BindAxis("Zoom", this, &AOverviewPlayer::Zoom);

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
	SpringArm->TargetArmLength += (AxisValue * 150);
}
