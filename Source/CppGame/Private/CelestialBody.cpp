// Copyright Soren Gilbertson


#include "CelestialBody.h"
#include "OrbitDebugActor.h"
#include "CelestialGameMode.h"

// Sets default values
ACelestialBody::ACelestialBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	//RootComponent = Root;

	if (Name == NAME_None)
	{
		Name = FName(this->GetName());
	}
}

void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();
	currentVelocity = initialVelocity;
	gameMode = Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode());
}

void ACelestialBody::RequestDestroyComponent(UActorComponent* ComponentToDestroy)
{
	ComponentToDestroy->DestroyComponent();
}

int ACelestialBody::SetMass(int newMass)
{
	mass = newMass; 

	if (AOrbitDebugActor::Get()->bAutoDraw)
	{
		AOrbitDebugActor::Get()->DrawOrbits();
	}

	return mass;
}

int ACelestialBody::GetMass() const { return mass; }

void ACelestialBody::UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep)
{
	for(int i = 0; i < allBodies.Num(); i++)
	{
		ACelestialBody* otherBody = allBodies[i];

		if (otherBody != this && gameMode != nullptr) {
			float sqrDst = (otherBody->GetActorLocation() - this->GetActorLocation()).Size();
			FVector forceDir = (otherBody->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
			FVector force = forceDir * gameMode->gravitationalConstant * mass * otherBody->GetMass() / sqrDst;
			FVector acceleration = force / mass;
			this->currentVelocity += acceleration * timeStep;
		}
	}
}

void ACelestialBody::UpdatePosition(float timeStep)
{
	this->SetActorLocation(FVector (this->GetActorLocation() + (currentVelocity * timeStep)));
	RootComponent->SetRelativeRotation(FRotator(RootComponent->GetRelativeRotation().Pitch, RootComponent->GetRelativeRotation().Yaw + (rotationRate / 100), RootComponent->GetRelativeRotation().Roll));
}

// Called every frame
void ACelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ACelestialBody::GetCurrentVelocity() const
{
	if (currentVelocity == FVector::ZeroVector)
	{
		return initialVelocity;
	}
	return currentVelocity;
}

void ACelestialBody::ScaleCurrentVelocity(float NewVelocity)
{
	FVector NormalizedVelocity = GetCurrentVelocity().GetSafeNormal();
	currentVelocity = NormalizedVelocity * NewVelocity;
	AOrbitDebugActor::Get()->DrawOrbits();
}

void ACelestialBody::SetCurrentVelocity(FVector NewVelocity)
{
	currentVelocity = NewVelocity;
}
