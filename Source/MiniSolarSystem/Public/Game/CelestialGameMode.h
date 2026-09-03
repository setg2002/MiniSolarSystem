// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Helpers/BodySystemFunctionLibrary.h"
#include "CelestialGameMode.generated.h"

/**
 * 
 */

//Forward Declarations
class ICelestialObject;
class ACelestialPlayer;
class AOverviewPlayer;
class ACelestialBody;
class ANiagaraActor;
class UUserWidget;
class APlanet;
class AStar;

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPerspectiveChanged, uint8, Perspective);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadingDone);


UCLASS()
class MINISOLARSYSTEM_API ACelestialGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec, BlueprintCallable)
	void ApplySettingsAssets();
private:
	TArray<class USettingsAsset*> SettingsAssets;

private:
	static float gravitationalConstant;

public:
	ACelestialGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FPerspectiveChanged OnPerspectiveChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FLoadingDone OnLoadingComplete;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	static float GetGravitationalConstant() { return gravitationalConstant; }
	UFUNCTION(BlueprintCallable)
	void SetGravitationalConstant(float NewG);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> OverviewWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> CelestialWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> OrbitDebugWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ANiagaraActor* AsteroidFieldActor;

	UFUNCTION(BlueprintCallable)
	bool SetAsteroidFieldActor(ANiagaraActor* NewAsteroidFieldActor);
	
	/* Sets the player perspective to the one desired
	* @param perspective 0 is overview, 1 is celestial */
	void SetPerspective(uint8 perspective);

	UFUNCTION(BlueprintCallable)
	uint8 GetCurrentPerspective() const { return currentPerspective; }
	
	void LoadGame();
	void SaveAsync(FAsyncSaveGameToSlotDelegate Out);

	// ======= Runtime Console Commands =======

	UFUNCTION(Exec, BlueprintCallable)
	void DeleteSave(int32 slot);

	UFUNCTION(Exec, BlueprintCallable)
	void SaveAndQuit();
	
	UFUNCTION(Exec, BlueprintCallable)
	void SaveAndQuitToMenu();

	UFUNCTION(Exec, BlueprintCallable)
	void Save() { SaveAsync(nullptr); }

	UFUNCTION(Exec, BlueprintCallable)
	void OrbitDebug();

	UFUNCTION(Exec, BlueprintCallable)
	void ReGenAll();

	UFUNCTION(Exec, BlueprintCallable)
	void ReGen(FString Planet);

	UFUNCTION(Exec, BlueprintCallable)
	void ReBind(FString Planet);

	UFUNCTION(Exec, BlueprintCallable)
	void tp(FString toPlanet);

	UFUNCTION(Exec, BlueprintCallable)
	void RemoveBody(FString Body);

	UFUNCTION(Exec, BlueprintCallable)
	void PauseGame();

	UFUNCTION(Exec, BlueprintCallable)
	void SetAsteroidFieldNum(int32 num); // Because you can't get niagara variable vlaues

	// ======= End ConsoleCommands =======

	UFUNCTION(BlueprintCallable)
	int32 GetAsteroidFieldNum() { return AsteroidFieldSpawnCount; }

	UFUNCTION(BlueprintCallable)
	ACelestialBody* AddBody(TSubclassOf<ACelestialBody> Class, FName Name, FTransform Transform, bool bRegenerate = true);
	
	UFUNCTION(BlueprintCallable)
	ANiagaraActor* AddAsteroidSystem(FTransform Transform, UNiagaraSystem* System, bool bStartPaused = true);
		
	UFUNCTION(BlueprintCallable)
	void RemoveAsteroidSystem(ANiagaraActor* SystemToRemove);
	
	// Duplicates the given body and returns the duplicated body
	UFUNCTION(BlueprintCallable)
	ACelestialBody* DuplicateBody(ACelestialBody* BodyToDuplicate);

	UFUNCTION(BlueprintCallable)
	ACelestialBody* GetBodyByName(FString Name);

	class ISettingsAssetID* GetAssetByID(uint32 ID);

	void LoadOnDiscAssetsOfClass(TArray<struct FDiscAssetRecord> LoadedAssets, TArray<FAssetData> OnDiscAssets, UClass* ClassToLoad);
	void LoadRuntimeAssetsOfClass(TArray<struct FAssetRecord> LoadedAssets, UClass* ClassToLoad);

private:
	UPROPERTY()
	TArray<FBodySystem> BodySystems;

public:
	UFUNCTION(BlueprintCallable)
	TArray<FBodySystem> GetBodySystems() const { return BodySystems; }
	UFUNCTION(BlueprintCallable)
	void AddBodySystem(UPARAM(ref) FBodySystem& NewSystem);
	UFUNCTION(BlueprintCallable)
	void RemoveBodySystem(UPARAM(ref) FBodySystem& System);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ACelestialBody*> Bodies;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AStar*> Stars;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ANiagaraActor*> Asteroids;
	
	TArray<ICelestialObject*> CelestialObjects;

	UPROPERTY() // Prevents garbage collection
	UUserWidget* OverviewWidget;
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* CelestialWidget;
	UPROPERTY()
	UUserWidget* PauseWidget;

	bool bGamePaused;

	uint8 currentPerspective;

	UPROPERTY()
	APlayerController* PC;

	UPROPERTY()
	ACelestialPlayer* CelestialPlayer;
	UPROPERTY()
	AOverviewPlayer* OverviewPlayer;

public:
	TArray<ACelestialBody*> GetBodies() { return Bodies; }
	TArray<AStar*> GetStars() { return Stars; }
	ACelestialPlayer* GetCelestialPlayer() const { return CelestialPlayer; }
	AOverviewPlayer* GetOverviewPlayer() const { return OverviewPlayer; }

private:
	UPROPERTY()
	UMaterialParameterCollectionInstance* PlanetIlluminationInst;

	// The spawn count for the asteroid field
	int32 AsteroidFieldSpawnCount;  // Because you can't get niagara variable values
	
	bool bHasInitAsteroidField = false;
};


namespace GeneratePlanetsOrdered
{
	static void DoGeneratePlanetsOrdered(TArray<FName> PlanetNames, ACelestialGameMode* GM);

	namespace // Anonymous namespace makes these variables "private"
	{
		static TArray<FName> TerrestrialPlanets;
		static TArray<FName> GeneratedPlanets;
		static ACelestialGameMode* GameMode;	
	}

	static bool bCurrentlyGenerating = false;
	
	static void NewGeneratedPlanet(FName PlanetName);
};
