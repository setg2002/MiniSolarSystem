// Copyright Soren Gilbertson


#include "CelestialBody.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CelestialGameMode.h"
#include "OrbitDebugActor.h"


// Sets default values
ACelestialBody::ACelestialBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ID = this->GetUniqueID();

	RootComponent = Collider = CreateDefaultSubobject<USphereComponent>(FName("RootCol"));
	Collider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();
	currentVelocity = initialVelocity;
	gameMode = Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode());

	Collider->OnComponentEndOverlap.AddDynamic(this, &ACelestialBody::OnOverlapEnd);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACelestialBody::OnOverlapBegin);
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
	FName UniqueName = EnsureUniqueName(NewName);
	if (UniqueName == NewName)
	{
		BodyName = NewName;
		return true;
	}
	else
	{
		BodyName = UniqueName;
		return false;
	}
}

FName ACelestialBody::EnsureUniqueName(FName InName)
{
	bool NameAlreadyExists = false;
	for (ACelestialBody* other : gameMode->GetBodies())
	{
		if (other->BodyName == InName)
		{
			NameAlreadyExists = true;
			break;
		}
	}
	if (NameAlreadyExists)
	{
		int32 i = 0;
		FName PersistantName = FName(FString(InName.ToString() + "_0"));
		while (true)
		{
			NameAlreadyExists = false;
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
				PersistantName = FName(FString(InName.ToString() + "_" + FString::FromInt(i)));
			}
			else return PersistantName;
		}
	}
	else return InName;
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
	ACelestialBody* OverlappedBody = Cast<ACelestialBody>(OverlappedActor);
	if (OverlappedActor && gameMode->GetCurrentPerspective() == 1 && OverlappedBody)
	{
		FVector CollisionNormal = (this->GetActorLocation() - OverlappedActor->GetActorLocation()).GetUnsafeNormal();
		currentVelocity += (CollisionNormal * ((currentVelocity.Size()) * ((OverlappedBody->mass * OverlappedBody->currentVelocity.Size()) / (this->mass * currentVelocity.Size()))));
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

void ACelestialBody::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) OverlappedActor = OtherActor; 
}

void ACelestialBody::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherActor == OverlappedActor)) OverlappedActor = nullptr;
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

void ACelestialBody::CalculateOrbitVelocity()
{
	if (OrbitingBody == this || OrbitingBody == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OrbitingBody cannot be null or itself"));
		return;
	}
	else
	{
		//TODO Replace 100 with gravitational constant from gamemode
		float GM = 100 * (this->mass + OrbitingBody->GetMass());
		orbitVelocity = FMath::Sqrt(GM);
		return;
	}
}

void ACelestialBody::SetToOrbit()
{
	if (OrbitingBody == this || OrbitingBody == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OrbitingBody cannot be null or itself"));
		return;
	}

	FVector AtPlanet = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), OrbitingBody->GetActorLocation()).Vector();
	FVector Up = Collider->GetUpVector();
	FVector Tangent = FVector().CrossProduct(AtPlanet, Up).GetSafeNormal();

	initialVelocity.X = Tangent.X * -orbitVelocity + OrbitingBody->initialVelocity.X;
	initialVelocity.Y = Tangent.Y * -orbitVelocity + OrbitingBody->initialVelocity.Y;
	initialVelocity.Z = Tangent.Z * -orbitVelocity + OrbitingBody->initialVelocity.Z;

	if (AOrbitDebugActor::Get()->bAutoDraw)
	{
		AOrbitDebugActor::Get()->DrawOrbits();
	}

	// Debugging
	if (bVectorDebug)
	{
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + (AtPlanet * VectorLength), VectorSize, FColor::Red, VectorDuration, VectorThickness);
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + (Up * VectorLength), VectorSize, FColor::Green, VectorDuration, VectorThickness);
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + (Tangent * VectorLength), VectorSize, FColor::Blue, VectorDuration, VectorThickness);
	}
}
