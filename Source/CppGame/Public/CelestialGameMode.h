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
class APlanet;
class AStar;

UCLASS()
class CPPGAME_API ACelestialGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACelestialGameMode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, CallInEditor)
	//void GenerateDistantStars();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float gravitationalConstant = 100;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<AStar> distantStarBP;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int numDistantStars;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int minSpawnRange = 50000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int maxSpawnRange = 200000;*/

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ACelestialBody*> bodies;

	//TArray<AStar*> distantStars;

	/*const FVector possibleColors[8] = { FVector(255, 255, 0),
									    FVector(255, 255, 184),
									    FVector(255, 255, 255),
									    FVector(0, 255, 255),
									    FVector(0, 147, 255),
									    FVector(0, 119, 255),
										FVector(0, 0, 255),
										FVector(255, 255, 255)
	};*/
};
