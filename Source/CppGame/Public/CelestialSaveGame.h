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


	// ----- Overview Player Data ----- \\

	UPROPERTY(VisibleAnywhere)
	int32 Speed;
	UPROPERTY(VisibleAnywhere)
	FRotator OverviewRotation;
	UPROPERTY(VisibleAnywhere)
	FVector OverviewLocation;
	UPROPERTY(VisibleAnywhere)
	float TargetArmLength;
	UPROPERTY(VisibleAnywhere)
	FRotator SpringArmRotation;


	// ----- Orbit Visualization Data ----- \\

	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<enum EDrawType> DrawType;
	UPROPERTY(VisibleAnywhere)
	int32 NumSteps;
	UPROPERTY(VisibleAnywhere)
	float TimeStep;
	UPROPERTY(VisibleAnywhere)
	bool bRelativeToBody;
	UPROPERTY(VisibleAnywhere)
	class ACelestialBody* CentralBody;
	UPROPERTY(VisibleAnywhere)
	float Width;
	UPROPERTY(VisibleAnywhere)
	int32 RenderedSteps;

};
