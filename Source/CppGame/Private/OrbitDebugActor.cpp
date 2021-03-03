// This is a copyright notice


#include "OrbitDebugActor.h"
#include "CelestialBody.h"
#include "Components/SplineComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AOrbitDebugActor::AOrbitDebugActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AOrbitDebugActor::OnConstruction(const FTransform & Transform)
{
	if (bDrawWithSplines)
	{
		CreateSplines();
	}

	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void AOrbitDebugActor::BeginPlay()
{
	Super::BeginPlay();
	
	ClearOrbits();
	if (bAutoDraw)
	{
		DrawOrbits();
	}
}

// Called every frame
void AOrbitDebugActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOrbitDebugActor::CreateSplines()
{
	// Clear current splines
	for (int i = 0; i < Splines.Num(); i++)
	{
		if (Splines[i])
		{
			Splines[i]->DestroyComponent();
		}
	}

	Splines.Empty();

	TArray<ACelestialBody*> Bodies;
	TArray<AActor*> CollectedActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACelestialBody::StaticClass(), CollectedActors);

	for (auto& body : CollectedActors)
	{
		Bodies.Add(Cast<ACelestialBody>(body));
	}

	for (int i = 0; i < Bodies.Num(); i++)
	{
		USplineComponent* NewSpline = NewObject<USplineComponent>(this, TEXT("Spline_" + i));

		Splines.Add(NewSpline);
		NewSpline->RegisterComponent();
		//NewSpline->OnComponentCreated();  // Crashes the editor
		NewSpline->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AOrbitDebugActor::DrawOrbits()
{
	ClearOrbits();

	TArray<ACelestialBody*> Bodies;
	TArray<AActor*> CollectedActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACelestialBody::StaticClass(), CollectedActors);

	for (auto& body : CollectedActors)
	{
		Bodies.Add(Cast<ACelestialBody>(body));
	}

	if (Bodies.Num() != Splines.Num() && bDrawWithSplines)
	{
		CreateSplines();
	}

	TArray<VirtualBody*> VirtualBodies;
	VirtualBodies.SetNum(Bodies.Num());

	TArray<TArray<FVector>> DrawPoints;
	DrawPoints.SetNum(VirtualBodies.Num());

	int ReferenceFrameIndex = 0;
	FVector ReferenceBodyInitialPosition = FVector::ZeroVector;

	// Initialize virtual bodies so we don't move the actual bodies
	for (int i = 0; i < VirtualBodies.Num(); i++)
	{
		VirtualBodies[i] = new VirtualBody(Bodies[i]);
		DrawPoints[i].SetNum(NumSteps);

		if (Bodies[i] == CentralBody && bRelativeToBody)
		{
			ReferenceFrameIndex = i;
			ReferenceBodyInitialPosition = VirtualBodies[i]->Position;
		}
	}

	// Simulate
	for (int step = 0; step < NumSteps; step++)
	{
		FVector ReferenceBodyPosition = (bRelativeToBody) ? VirtualBodies[ReferenceFrameIndex]->Position : FVector::ZeroVector;
		// Update velocity
		for (int i = 0; i < VirtualBodies.Num(); i++)
		{
			VirtualBodies[i]->Velocity += CalculateAcceleration(i, VirtualBodies) * TimeStep;
		}
		// Update positions
		for (int i = 0; i < VirtualBodies.Num(); i++)
		{
			FVector newPos = VirtualBodies[i]->Position + VirtualBodies[i]->Velocity * TimeStep;
			VirtualBodies[i]->Position = newPos;
			if (bRelativeToBody)
			{
				auto referenceFrameOffset = ReferenceBodyPosition - ReferenceBodyInitialPosition;
				newPos -= referenceFrameOffset;
			}
			if (bRelativeToBody && i == ReferenceFrameIndex)
			{
				newPos = ReferenceBodyInitialPosition;
			}

			DrawPoints[i][step] = newPos;
		}
	}

	// Draw paths
	for (int bodyIndex = 0; bodyIndex < VirtualBodies.Num(); bodyIndex++)
	{
		if (bDrawWithSplines)
		{
			TArray<FSplinePoint> SplinePoints;
			for (int i = 2; i < DrawPoints[bodyIndex].Num(); i++)
			{
				SplinePoints.Add(FSplinePoint(i, DrawPoints[bodyIndex][i]));
			}

			Splines[bodyIndex]->AddPoints(SplinePoints);
			Splines[bodyIndex]->EditorUnselectedSplineSegmentColor = Colors[bodyIndex];
			Splines[bodyIndex]->SetDrawDebug(true);
		}

		else
		{
			for (int i = 0; i < DrawPoints[bodyIndex].Num() - 1; i++)
				{
					DrawDebugLine(GetWorld(), DrawPoints[bodyIndex][i], DrawPoints[bodyIndex][i + 1], Colors[bodyIndex], true, 0.f, 0, Width);
				}
		}
		
	}
}

void AOrbitDebugActor::ClearOrbits()
{
	TArray<AActor*> CollectedActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACelestialBody::StaticClass(), CollectedActors);
	for (int bodyIndex = 0; bodyIndex < CollectedActors.Num(); bodyIndex++)
	{
		if (Splines[bodyIndex])
		{
			Splines[bodyIndex]->ClearSplinePoints();
		}
	}
	FlushPersistentDebugLines(GetWorld());
}

FVector AOrbitDebugActor::CalculateAcceleration(int i, TArray<VirtualBody*> VirtualBodies) {
	FVector Acceleration = FVector::ZeroVector;
	for (int j = 0; j < VirtualBodies.Num(); j++)
	{
		if (i == j)
		{
			continue;
		}

		FVector ForceDir = (VirtualBodies[j]->Position - VirtualBodies[i]->Position).GetSafeNormal();
		float sqrDst = (VirtualBodies[j]->Position - VirtualBodies[i]->Position).Size();
		//TODO Use actual gravitational constant
		Acceleration += ForceDir * 100 /*Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode())->gravitationalConstant*/ * VirtualBodies[j]->Mass / sqrDst;
	}
	return Acceleration;
}

VirtualBody::VirtualBody(ACelestialBody* Body)
{
	Position = Body->GetActorLocation();
	Velocity = Body->initialVelocity;
	Mass = Body->mass;
}

void AOrbitDebugActor::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if ((PropertyName == GET_MEMBER_NAME_CHECKED(AOrbitDebugActor, NumSteps) || PropertyName == GET_MEMBER_NAME_CHECKED(AOrbitDebugActor, TimeStep) || PropertyName == GET_MEMBER_NAME_CHECKED(AOrbitDebugActor, bRelativeToBody) || PropertyName == GET_MEMBER_NAME_CHECKED(AOrbitDebugActor, CentralBody) || PropertyName == GET_MEMBER_NAME_CHECKED(AOrbitDebugActor, Width)) && bAutoDraw)
		{
			DrawOrbits();
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
