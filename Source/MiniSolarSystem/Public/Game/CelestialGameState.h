// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CelestialGameState.generated.h"

class AOverviewPlayer;
class ACelestialPlayer;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API ACelestialGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
protected:
	// Cached references
	
	UPROPERTY(BlueprintReadOnly)
	ACelestialPlayer* CelestialPlayer;
	
	UPROPERTY(BlueprintReadOnly)
	AOverviewPlayer* OverviewPlayer;
	
	
protected:
	/** MappingContext for player input. */
	UPROPERTY(BlueprintReadOnly)
	UInputMappingContext* InputMappingContext_Celestial;
	
	UPROPERTY(BlueprintReadOnly)
	UInputMappingContext* InputMappingContext_Overview;
	
	UPROPERTY(BlueprintReadOnly)
	UInputMappingContext* InputMappingContext_Controller;
	
public:
	UInputMappingContext* GetCelestialInputMappingContext() const { return InputMappingContext_Celestial; }
	UInputMappingContext* GetOverviewInputMappingContext() const { return InputMappingContext_Overview; }
	UInputMappingContext* GetControllerInputMappingContext() const { return InputMappingContext_Controller; }
	
	void SetCelestialMappingContext(UInputMappingContext* NewMappingContext) { InputMappingContext_Celestial = NewMappingContext; }
	void SetOverviewMappingContext(UInputMappingContext* NewMappingContext) { InputMappingContext_Overview = NewMappingContext; }
	void SetControllerMappingContext(UInputMappingContext* NewMappingContext) { InputMappingContext_Controller = NewMappingContext; }
};
