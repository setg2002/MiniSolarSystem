// This is a copyright notice


#include "OverviewPlayer.h"
#include "CelestialGameMode.h"

// Sets default values
AOverviewPlayer::AOverviewPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	PlayerInputComponent->BindAxis("MoveRight", this, &AOverviewPlayer::MoveRight);
}

void AOverviewPlayer::SwitchPerspective()
{
	gameMode->SetPerspective(1);
}

