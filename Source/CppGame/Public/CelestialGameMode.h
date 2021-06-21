// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "OrbitDebugActor.h"
#include "GameFramework/GameModeBase.h"
#include "CelestialGameMode.generated.h"

/**
 * 
 */

//Forward Declarations
class ICelestialObject;
class ACelestialPlayer;
class AOverviewPlayer;
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
	TSubclassOf<UUserWidget> HighlightWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> OverviewWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> CelestialWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> OrbitDebugWidgetClass;

	/* Sets the player perspective to the one desired
	* @param perspective 0 is overview, 1 is celestial */
	void SetPerspective(uint8 perspective);
	
	void LoadGame();

	// ======= Runtime Console Commands =======

	UFUNCTION(Exec, BlueprintCallable)
	void DeleteSave();

	UFUNCTION(Exec, BlueprintCallable)
	void SaveAndQuit();
	
	UFUNCTION(Exec, BlueprintCallable)
	void SaveAndQuitToMenu();

	UFUNCTION(Exec, BlueprintCallable)
	void Save();

	UFUNCTION(Exec, BlueprintCallable)
	void OrbitDebug();

	UFUNCTION(Exec, BlueprintCallable)
	void ReGenAll();

	UFUNCTION(Exec, BlueprintCallable)
	void ReGen(FString Planet);

	UFUNCTION(Exec, BlueprintCallable)
	void tp(FString toPlanet);

	UFUNCTION(Exec, BlueprintCallable)
	void SetTerrainResolution(FString Planet, int32 resolution);

	// ======= End ConsoleCommands =======

	// For Loading
	UPROPERTY(BlueprintReadOnly)
	TArray<FName> TerrestrialPlanets;
	UPROPERTY(BlueprintReadOnly)
	TArray<FName> GeneratedPlanets;

	UFUNCTION()
	void NewGeneratedPlanet(FName PlanetName);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ACelestialBody*> bodies;

	TArray<ICelestialObject*> celestialObjects;

	ACelestialBody* GetBodyByName(FString Name);

	UPROPERTY() // Prevents garbage collection
	UUserWidget* OverviewWidget;
	UPROPERTY()
	UUserWidget* CelestialWidget;

	uint8 currentPerspective;

	UPROPERTY()
	APlayerController* PC;

	UPROPERTY()
	ACelestialPlayer* CelestialPlayer;
	UPROPERTY()
	AOverviewPlayer* OverviewPlayer;

public:
	TArray<ACelestialBody*> GetBodies() { return bodies; }

private:
	bool b = true; // dumb
};
