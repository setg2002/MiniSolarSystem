// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbitDebugActor.generated.h"

// Forward Declarations
class ACelestialBody;
class USplineComponent;
class UNiagaraSystem;
class UNiagaraComponent;


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
	// Sets default values for this actor's properties
	AOrbitDebugActor();

	UPROPERTY(EditAnywhere)
	bool bAutoDraw = true;

	UPROPERTY(EditAnywhere)
	bool bDrawWithSplines = true;
	
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

	UPROPERTY(EditAnywhere)
	float Width = 25;

	UPROPERTY(EditAnywhere)
	TArray<USplineComponent*> Splines;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void DrawOrbits();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void ClearOrbits();

	void CreateSplines();

	virtual void OnConstruction(const FTransform & Transform) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UNiagaraSystem* ParticleTemplate;

	TArray<UNiagaraComponent*> ParticleComponents;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	TArray<FColor> Colors = { FColor::Red, FColor::Blue, FColor::Green, FColor::White, FColor::Magenta, FColor::Cyan };

	FVector CalculateAcceleration(int i, TArray<VirtualBody*> VirtualBodies);
};
