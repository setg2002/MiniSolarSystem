// Copyright Soren Gilbertson


#include "CelestialBody.h"
#include "Components/WidgetComponent.h"
#include "CelestialGameMode.h"
#include "OrbitDebugActor.h"


// Sets default values
ACelestialBody::ACelestialBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
}

void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();
	currentVelocity = initialVelocity;
	gameMode = Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode());

	// Ensure unique name
	int32 i = 0;
	FName PersistantName = FName(FString(this->GetBodyName().ToString() + "_0"));
	while (BodyName == NAME_None)
	{
		bool NameAlreadyExists = false;
		for (ACelestialBody* other : gameMode->GetBodies())
		{
			if (other->BodyName == PersistantName)
			{
				NameAlreadyExists = true;
				break;
			}
		}
		if (NameAlreadyExists)
		{
			i++;
			PersistantName = FName(FString(this->GetBodyName().ToString() + "_" + FString::FromInt(i)));
		}
		else
			BodyName = PersistantName;
	}
}

void ACelestialBody::RequestDestroyComponent(UActorComponent* ComponentToDestroy)
{
	ComponentToDestroy->DestroyComponent();
}

int ACelestialBody::SetMass(int newMass)
{
	mass = newMass; 

	if (AOrbitDebugActor::Get()->bAutoDraw)
		AOrbitDebugActor::Get()->DrawOrbits();

	return mass;
}

int ACelestialBody::GetMass() const { return mass; }

bool ACelestialBody::SetName(FName NewName)
{
	for (ACelestialBody* other : gameMode->GetBodies())
	{
		if (other->BodyName == NewName)
		{
			return false;
		}
	}
	BodyName = NewName;
	return true;
}

void ACelestialBody::UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep)
{
	for(int i = 0; i < allBodies.Num(); i++)
	{
		ACelestialBody* otherBody = allBodies[i];

		if (otherBody != this && gameMode != nullptr) 
		{
			float sqrDst = (otherBody->GetActorLocation() - this->GetActorLocation()).Size();
			FVector forceDir = (otherBody->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
			FVector force = forceDir * ACelestialGameMode::GetGravitationalConstant() * mass * otherBody->GetMass() / sqrDst;
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

void ACelestialBody::SetLocation(FVector NewLocation)
{
	this->SetActorLocation(NewLocation);
	AOrbitDebugActor::Get()->DrawOrbits();
}

// Called every frame
void ACelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ACelestialBody::GetCurrentVelocity() const
{
	if (currentVelocity == FVector::ZeroVector)
		return initialVelocity;

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

void ACelestialBody::AddCelestialComponent(UStaticMeshComponent* NewComp)
{
	if (!NewComp->IsRegistered())
		NewComp->RegisterComponent();
	NewComp->AttachToComponent(this->RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}
