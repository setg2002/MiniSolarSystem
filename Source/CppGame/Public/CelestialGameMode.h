// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CelestialBody.h"
#include "CelestialGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CPPGAME_API ACelestialGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACelestialGameMode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	float gravitationalConstant;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	TArray<ACelestialBody*> bodies;
};
