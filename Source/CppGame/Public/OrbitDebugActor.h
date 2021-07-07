// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbitDebugActor.generated.h"

// Forward Declarations
class ACelestialBody;
class USplineComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UENUM(BlueprintType)
enum EDrawType
{
	DebugLine,
	Spline,
	Ribbon
};

class VirtualBody
{
public:
	FVector Position;
	FVector Velocity;
	int Mass;

	VirtualBody(ACelestialBody* Body);
};

UCLASS()
class CPPGAME_API AOrbitDebugActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawType> DrawType;

	UPROPERTY(SaveGame, EditAnywhere)
	int32 NumSteps = 1000;

	UPROPERTY(SaveGame, EditAnywhere)
	float TimeStep = 0.0833f;

	UPROPERTY(SaveGame, EditAnywhere)
	bool bRelativeToBody;

	UPROPERTY(SaveGame, EditAnywhere)
	ACelestialBody* CentralBody;

	// The width of the debug. If it is 0 (zero) then the planet's radius will be used
	UPROPERTY(SaveGame, EditAnywhere)
	float Width = 25;

	UPROPERTY(SaveGame, EditAnywhere)
	int32 RenderedSteps = 100;

public:	
	UFUNCTION(BlueprintCallable)
	static AOrbitDebugActor* Get();

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EDrawType> GetDrawType() const { return DrawType; }
	UFUNCTION(BlueprintCallable)
	void SetDrawType(TEnumAsByte<EDrawType> NewDrawType);

	UFUNCTION(BlueprintCallable)
	int32 GetNumSteps() const { return NumSteps; }
	UFUNCTION(BlueprintCallable)
	void SetNumSteps(int32 NewNumSteps);

	UFUNCTION(BlueprintCallable)
	int32 GetRenderedSteps() const { return RenderedSteps; }
	UFUNCTION(BlueprintCallable)
	void SetRenderedSteps(int32 NewRenderedSteps);

	UFUNCTION(BlueprintCallable)
	float GetTimeStep() const { return TimeStep; }
	UFUNCTION(BlueprintCallable)
	void SetTimeStep(float NewTimeStep);

	UFUNCTION(BlueprintCallable)
	float GetWidth() const { return Width; }
	UFUNCTION(BlueprintCallable)
	void SetWidth(float NewWidth);

	UFUNCTION(BlueprintCallable)
	bool GetRelativeToBody() const { return bRelativeToBody; }
	UFUNCTION(BlueprintCallable)
	void SetRelativeToBody(bool NewRelativeToBody);

	UFUNCTION(BlueprintCallable)
	ACelestialBody* GetRelativeBody() const { return CentralBody; }
	UFUNCTION(BlueprintCallable)
	void SetRelativeBody(ACelestialBody* NewRelativeBody);

	UPROPERTY(EditAnywhere)
	bool bAutoDraw = true;

	UPROPERTY(SaveGame, EditAnywhere)
	bool bUsePhysicsTimeStep;

	UPROPERTY(EditAnywhere)
	TArray<USplineComponent*> Splines;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void DrawOrbits();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void ClearOrbits();

	virtual void OnConstruction(const FTransform & Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> ARRAY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 aaaaaa;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UNiagaraSystem* ParticleTemplate;

	TArray<UNiagaraComponent*> ParticleComponents;

	void CreateSplines();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	// Sets default values for this actor's properties
	AOrbitDebugActor();
	
	static AOrbitDebugActor* _instance;

	TArray<FColor> Colors = { FColor::Red, FColor::Blue, FColor::Green, FColor::White, FColor::Magenta, FColor::Cyan, FColor::Emerald, FColor::Orange, FColor::Purple, FColor::Silver, FColor::Yellow };

	FVector CalculateAcceleration(int i, TArray<VirtualBody*> VirtualBodies);
};
