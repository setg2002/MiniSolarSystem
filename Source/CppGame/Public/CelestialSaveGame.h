// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CelestialSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CPPGAME_API UCelestialSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UCelestialSaveGame();

	UPROPERTY(VisibleAnywhere)
	TMap<class ACelestialBody*, FVector> CelestialLocations;

	UPROPERTY(VisibleAnywhere)
	TMap<class ACelestialBody*, FVector> CelestialVelocities;


	// ----- Celestial Player Data ----- \\

	UPROPERTY(VisibleAnywhere)
	FVector PlayerLocation;
	UPROPERTY(VisibleAnywhere)
	FVector PlayerVelocity;
	UPROPERTY(VisibleAnywhere)
	FRotator PlayerRotation;

	UPROPERTY(VisibleAnywhere)
	bool IgnoreGravity;
	UPROPERTY(VisibleAnywhere)
	float Throttle;

};
