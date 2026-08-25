// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CelestialPlayerController.generated.h"


class UInputMappingContext;
class UInputDataConfig_Controller;
class ACelestialGameMode;

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API ACelestialPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACelestialPlayerController();
	
protected:
	
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputDataConfig_Controller* ControllerInputConfig;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowChangePerspective;
	
	UPROPERTY(BlueprintReadOnly)
	ACelestialGameMode* gameMode;
	
	UFUNCTION(Exec)
	void SwitchPerspective();
};
