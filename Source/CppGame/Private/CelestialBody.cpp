// This is a copyright notice


#include "CelestialBody.h"
#include "CelestialGameMode.h"

// Sets default values
ACelestialBody::ACelestialBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	//RootComponent = Root;
}

void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();
	currentVelocity = initialVelocity;
	gameMode = Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode());
}

void ACelestialBody::UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep)
{
	for(int i = 0; i < allBodies.Num(); i++)
	{
		ACelestialBody* otherBody = allBodies[i];

		if (otherBody != this && gameMode != nullptr) {
			float sqrDst = (otherBody->GetActorLocation() - this->GetActorLocation()).Size();
			FVector forceDir = (otherBody->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
			FVector force = forceDir * gameMode->gravitationalConstant * this->mass * otherBody->mass / sqrDst;
			FVector acceleration = force / this->mass;
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

