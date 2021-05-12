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

UENUM()
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
	
public:	
	static AOrbitDebugActor* Get();

	UPROPERTY(EditAnywhere)
	bool bAutoDraw = true;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawType> DrawType;
	
	UPROPERTY(EditAnywhere)
	int NumSteps = 1000;

	UPROPERTY(EditAnywhere)
	float TimeStep = 0.0833f;

	UPROPERTY(EditAnywhere)
	bool bUsePhysicsTimeStep;

	UPROPERTY(EditAnywhere)
	bool bRelativeToBody;

	UPROPERTY(EditAnywhere)
	ACelestialBody* CentralBody;

	// The width of the debug. If it is 0 (zero) then the planet's radius will be used
	UPROPERTY(EditAnywhere)
	float Width = 25;

	UPROPERTY(EditAnywhere)
	int RenderedSteps = 100;

	UPROPERTY(EditAnywhere)
	TArray<USplineComponent*> Splines;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void DrawOrbits();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void ClearOrbits();

	virtual void OnConstruction(const FTransform & Transform) override;

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

	TArray<FColor> Colors = { FColor::Red, FColor::Blue, FColor::Green, FColor::White, FColor::Magenta, FColor::Cyan };

	FVector CalculateAcceleration(int i, TArray<VirtualBody*> VirtualBodies);
};
