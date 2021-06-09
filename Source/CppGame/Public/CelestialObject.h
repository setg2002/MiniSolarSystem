// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CelestialObject.generated.h"

class ACelestialBody;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCelestialObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CPPGAME_API ICelestialObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep) { }

	UFUNCTION()
	virtual void UpdatePosition(float timeStep) { }

	UFUNCTION()
	virtual FVector GetCurrentVelocity() const { return FVector::ZeroVector; }

	UFUNCTION()
	virtual int GetMass() const { return 0; }
};
