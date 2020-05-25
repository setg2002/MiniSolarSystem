// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CelestialGameMode.generated.h"

/**
 * 
 */

//Forward Declarations
class ACelestialBody;

UCLASS()
class CPPGAME_API ACelestialGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACelestialGameMode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	float gravitationalConstant = 100;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ACelestialBody*> bodies;
};
