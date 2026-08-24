// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CelestialGameState.generated.h"

class AOverviewPlayer;
class ACelestialPlayer;
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
};
