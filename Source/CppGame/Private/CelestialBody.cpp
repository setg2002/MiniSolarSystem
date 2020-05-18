// This is a copyright notice


#include "CelestialBody.h"

// Sets default values
ACelestialBody::ACelestialBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();

	currentVelocity = initialVelocity;
}

void ACelestialBody::UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep)
{
	for(int i = 0; i < allBodies.Num(); i++)
	{
		ACelestialBody* otherBody = allBodies[i];

		if (otherBody != this) {
			float sqrDst = (otherBody->GetActorLocation() - this->GetActorLocation()).SizeSquared();
			FVector forceDir = (otherBody->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
			FVector force = forceDir * 1 * this->mass * otherBody->mass / sqrDst;       //TODO make the number whith a gamemode variable: gravitationalConstant
			FVector acceleration = force / this->mass;
			this->currentVelocity += acceleration * timeStep;
		}
	}
}

void ACelestialBody::UpdatePosition(float timeStep)
{
	this->SetActorLocation(FVector (this->GetActorLocation() += currentVelocity * timeStep));
}


// Called every frame
void ACelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

