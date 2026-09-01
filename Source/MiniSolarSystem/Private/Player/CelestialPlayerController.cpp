// Copyright Soren Gilbertson


#include "Player/CelestialPlayerController.h"

#include "Game/CelestialGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/InputDataConfig.h"
#include "Helpers/BodySystemFunctionLibrary.h"
#include "Game/CelestialGameState.h"

ACelestialPlayerController::ACelestialPlayerController()
{
	bAllowChangePerspective = false;

	if (GetWorld())
	{
		gameMode = Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode());
	}
}

void ACelestialPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACelestialPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    
	ACelestialGameState* GameState = GetWorld()->GetGameState<ACelestialGameState>();
	
	if (!GameState) return;
	
	GameState->SetControllerMappingContext(InputMappingContext);
	Subsystem->AddMappingContext(InputMappingContext, 1);
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
    
	if(!ControllerInputConfig) return; // Don't wanna bind to invalid UInputActions!
    
	Input->BindAction(ControllerInputConfig->SwitchPerspective, ETriggerEvent::Completed, this, &ACelestialPlayerController::SwitchPerspective);
}

void ACelestialPlayerController::SwitchPerspective()
{
	if (!IsValid(gameMode))
	{
		return;
	}
	
	if (bAllowChangePerspective)
	{
		gameMode->SetPerspective(!gameMode->GetCurrentPerspective());
	}
}
