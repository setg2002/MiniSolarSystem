// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CelestialGameMode.generated.h"

/**
 * 
 */

//Forward Declarations
class ICelestialObject;
class ACelestialBody;
class UUserWidget;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float gravitationalConstant = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> OverviewWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> CelestialWidgetClass;

	/* Sets the player perspective to the one desired
	* @param perspective 0 is overview, 1 is celestial */
	void SetPerspective(uint8 perspective);
	
	// ======= Runtime Console Commands =======

	UFUNCTION(Exec)
	void ReGenAll();

	UFUNCTION(Exec)
	void ReGen(FString Planet);

	UFUNCTION(Exec)
	void tp(FString toPlanet);

	// ======= End ConsoleCommands =======

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ACelestialBody*> bodies;

	TArray<ICelestialObject*> celestialObjects;

	UUserWidget* OverviewWidget;
	UUserWidget* CelestialWidget;

	uint8 currentPerspective;

	APlayerController* PC;
};
