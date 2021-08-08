	// Copyright Soren Gilbertson


#include "OrbitDebugActor.h"
#include "Star.h"
#include "Planet.h"
#include "GasGiant.h"
#include "CelestialBody.h"
#include "NiagaraSystem.h"
#include "ShapeSettings.h"
#include "NiagaraComponent.h"
#include "DrawDebugHelpers.h"
#include "CelestialGameMode.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SplineComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


AOrbitDebugActor* AOrbitDebugActor::_instance; // Needed for singleton implementation

AOrbitDebugActor* AOrbitDebugActor::Get()
{
	if (_instance == 0)
	{
		_instance = NewObject<AOrbitDebugActor>();
	}
	return _instance;
}

// Sets default values
AOrbitDebugActor::AOrbitDebugActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_instance = this; //FIX Possibly redundant from Get()?

	ParticleTemplate = LoadObject<UNiagaraSystem>(NULL, TEXT("NiagaraSystem'/Game/Particles/OrbitDebug/OrbitDebugEmitter_System.OrbitDebugEmitter_System'"), NULL, LOAD_None, NULL);
}

void AOrbitDebugActor::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void AOrbitDebugActor::BeginPlay()
{
	Super::BeginPlay();
	
	ClearOrbits();
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

	Splines.SetNum(Bodies.Num());

	for (int i = 0; i < Bodies.Num(); i++)
	{
		FString Name = "Spline_";
		Name.Append(FString::FromInt(i));
		USplineComponent* NewSpline = NewObject<USplineComponent>(this, FName(Name));

		Splines[i] = NewSpline;
		NewSpline->RegisterComponent();
		//NewSpline->OnComponentCreated();  // Crashes the editor
		NewSpline->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AOrbitDebugActor::DrawOrbits()
{
	ClearOrbits();
	
	TArray<ACelestialBody*> Bodies;
#if WITH_EDITORONLY_DATA 
	TArray<AActor*> CollectedActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACelestialBody::StaticClass(), CollectedActors);
	for (auto& body : CollectedActors)
	{
		Bodies.Add(Cast<ACelestialBody>(body));
	}
#else
	Bodies = Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode())->GetBodies();
#endif

	if (Bodies.Num() != Splines.Num() && DrawType == EDrawType::Spline)
	{
		CreateSplines();
	}

	TArray<VirtualBody*> VirtualBodies;
	VirtualBodies.SetNum(Bodies.Num());

	TArray<TArray<FVector>> DrawPoints;
	DrawPoints.SetNum(VirtualBodies.Num());

	if (DrawType == EDrawType::Ribbon)
	{
		ParticleComponents.Empty();
		ParticleComponents.SetNum(VirtualBodies.Num());
		for (int i = 0; i < VirtualBodies.Num(); i++)
		{
			ParticleComponents[i] = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParticleTemplate, this->GetActorLocation());
		}
	}

	// If the central body got deleted we don't want the debug to be relative
	if (!CentralBody || (CentralBody->IsPendingKill()))
	{
		bRelativeToBody = false;
		CentralBody = Bodies[0];
	}

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
	FVector ReferenceBodyPosition = (bRelativeToBody) ? VirtualBodies[ReferenceFrameIndex]->Position : FVector::ZeroVector;
	for (int i = 0; i < VirtualBodies.Num(); i++) // Makes sure the 1st position of the orbit is at the planet's exact position
	{
		if (bRelativeToBody)
		{
			DrawPoints[i][0] = VirtualBodies[i]->Position - (ReferenceBodyPosition - ReferenceBodyInitialPosition);
		}
		else
		{
			DrawPoints[i][0] = VirtualBodies[i]->Position;
		}
	}
	for (int step = 1; step < NumSteps; step++)
	{
		ReferenceBodyPosition = (bRelativeToBody) ? VirtualBodies[ReferenceFrameIndex]->Position : FVector::ZeroVector;

		// Update velocity
		for (int i = 0; i < VirtualBodies.Num(); i++)
		{
			VirtualBodies[i]->Velocity += (CalculateAcceleration(i, VirtualBodies) * TimeStep);
		}
		// Update positions
		for (int i = 0; i < VirtualBodies.Num(); i++)
		{
			FVector newPos = VirtualBodies[i]->Position + (VirtualBodies[i]->Velocity * TimeStep);
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
		switch (DrawType)
		{
		case DebugLine:
		{
			int factor = NumSteps / (RenderedSteps * 10) < 1 ? 1 : NumSteps / (RenderedSteps * 10); // Scale down the number of lines to use as NumSteps grows over RenderedSteps to retain framerate
			for (int i = 0; i < FMath::Min(DrawPoints[bodyIndex].Num() - 1, (RenderedSteps * 10) - 1); i++)
			{
				APlanet* planet = Cast<APlanet>(Bodies[bodyIndex]);
				if (Width == 0 && planet != nullptr)
				{
					DrawDebugLine(GetWorld(), DrawPoints[bodyIndex][i * factor], DrawPoints[bodyIndex][(i + 1) * factor], Colors[bodyIndex], true, 0.f, 0, planet->ShapeSettings->GetRadius() * 2);
				}
				else if (Width == 0 && Cast<AGasGiant>(Bodies[bodyIndex]))
				{
					DrawDebugLine(GetWorld(), DrawPoints[bodyIndex][i * factor], DrawPoints[bodyIndex][(i + 1) * factor], Colors[bodyIndex], true, 0.f, 0, Cast<AGasGiant>(Bodies[bodyIndex])->GetRadius() * 200);
				}
				else
				{
					DrawDebugLine(GetWorld(), DrawPoints[bodyIndex][i * factor], DrawPoints[bodyIndex][(i + 1) * factor], Colors[bodyIndex], true, 0.f, 0, Width);
				}
			}
			break;
		}
		case Spline:
		{
			int factor = NumSteps / RenderedSteps < 1 ? 1 : NumSteps / RenderedSteps;
			// Add more points to the end of the spline until we reach NumSteps
			if (Splines[bodyIndex]->GetNumberOfSplinePoints() < FMath::Min(DrawPoints[bodyIndex].Num() - 1, RenderedSteps - 1))
			{
				for (int i = Splines[bodyIndex]->GetNumberOfSplinePoints(); i < FMath::Min(DrawPoints[bodyIndex].Num() - 1, RenderedSteps - 1); i++)
				{
					Splines[bodyIndex]->AddPoint(FSplinePoint(i, DrawPoints[bodyIndex][i * factor]), false);
				}
			}
			// Remove points from the end of the spline until we reach NumSteps
			else
			{
				while (Splines[bodyIndex]->GetNumberOfSplinePoints() > NumSteps)
				{
					Splines[bodyIndex]->RemoveSplinePoint(Splines[bodyIndex]->GetNumberOfSplinePoints() - 1, false);
				}
			}

			Splines[bodyIndex]->SetDrawDebug(true);

#if WITH_EDITOR
			Splines[bodyIndex]->EditorUnselectedSplineSegmentColor = Colors[bodyIndex];
			Splines[bodyIndex]->bShouldVisualizeScale = true;
			APlanet* planet = Cast<APlanet>(Bodies[bodyIndex]);
			if (planet != nullptr)
			{
				Splines[bodyIndex]->ScaleVisualizationWidth = planet->ShapeSettings->GetRadius();
			}
#endif
			Splines[bodyIndex]->UpdateSpline();
			break;
		}
		case Ribbon:
		{
			RenderedSteps = FMath::Clamp<int32>(0.0005f * NumSteps * NumSteps, 0, 5000);
			TArray<FVector> NewPoints;
			int factor = NumSteps / RenderedSteps < 1 ? 1 : NumSteps / RenderedSteps; // Scale down the number of lines to use as NumSteps grows over RenderedSteps to retain framerate
			for (int j = 0; j < FMath::Min(DrawPoints[bodyIndex].Num() - 1, RenderedSteps - 1); j++)
			{
				NewPoints.Add(DrawPoints[bodyIndex][j * factor]);
			}

			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(ParticleComponents[bodyIndex], FName("User.Points"), NewPoints);
			ParticleComponents[bodyIndex]->SetColorParameter(FName("User.Color"), Colors[bodyIndex % Colors.Num()]);

			if (Width == 0)
			{
				if (APlanet* Planet = Cast<APlanet>(Bodies[bodyIndex]))
				{
					ParticleComponents[bodyIndex]->SetFloatParameter(FName("User.Width"), Planet->ShapeSettings->GetRadius() * 2);
				}
				else if (AGasGiant* GasGiant = Cast<AGasGiant>(Bodies[bodyIndex]))
				{
					ParticleComponents[bodyIndex]->SetFloatParameter(FName("User.Width"), GasGiant->GetRadius() * 200);
				}
				else if (AStar * Star = Cast<AStar>(Bodies[bodyIndex]))
				{
					ParticleComponents[bodyIndex]->SetFloatParameter(FName("User.Width"), Star->starProperties.radius * 100);
				}
			}
			else
			{
				ParticleComponents[bodyIndex]->SetFloatParameter(FName("User.Width"), Width * 20);
			}
			break;
		}
		default:
			break;
		}
	}
}

void AOrbitDebugActor::ClearOrbits()
{
	for (int i = 0; i < ParticleComponents.Num(); i++)
	{
		if (ParticleComponents[i]->IsValidLowLevel())
		{
			ParticleComponents[i]->DestroyComponent();
		}
	}
	ParticleComponents.Empty();

	TArray<AActor*> CollectedActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACelestialBody::StaticClass(), CollectedActors);
	for (int bodyIndex = 0; bodyIndex < CollectedActors.Num(); bodyIndex++)
	{
		if (Splines.IsValidIndex(bodyIndex))
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
		Acceleration += ForceDir * ACelestialGameMode::GetGravitationalConstant() * VirtualBodies[j]->Mass / sqrDst;
	}
	return Acceleration;
}

VirtualBody::VirtualBody(ACelestialBody* Body)
{
	Position = Body->GetActorLocation();
	Velocity = Body->GetCurrentVelocity();
	Mass = Body->GetMass();
}


void AOrbitDebugActor::SetDrawType(TEnumAsByte<EDrawType> NewDrawType)
{
	DrawType = NewDrawType;
	DrawOrbits();
}

void AOrbitDebugActor::SetNumSteps(int32 NewNumSteps)
{
	NumSteps = NewNumSteps;
	DrawOrbits();
}

void AOrbitDebugActor::SetRenderedSteps(int32 NewRenderedSteps)
{
	RenderedSteps = NewRenderedSteps;
	DrawOrbits();
}

void AOrbitDebugActor::SetTimeStep(float NewTimeStep)
{
	TimeStep = NewTimeStep;
	DrawOrbits();
}

void AOrbitDebugActor::SetWidth(float NewWidth)
{
	Width = NewWidth;
	DrawOrbits();
}

void AOrbitDebugActor::SetRelativeToBody(bool NewRelativeToBody)
{
	bRelativeToBody = NewRelativeToBody;
	DrawOrbits();
}

void AOrbitDebugActor::SetRelativeBody(ACelestialBody* NewRelativeBody)
{
	CentralBody = NewRelativeBody;
	DrawOrbits();
}


#if WITH_EDITOR
void AOrbitDebugActor::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		//TODO Is there a shorter way to rewrite this line?
		if ((PropertyName == GET_MEMBER_NAME_CHECKED(AOrbitDebugActor, DrawType) || PropertyName == GET_MEMBER_NAME_CHECKED(AOrbitDebugActor, NumSteps) || PropertyName == GET_MEMBER_NAME_CHECKED(AOrbitDebugActor, TimeStep) || PropertyName == GET_MEMBER_NAME_CHECKED(AOrbitDebugActor, bRelativeToBody) || PropertyName == GET_MEMBER_NAME_CHECKED(AOrbitDebugActor, CentralBody) || PropertyName == GET_MEMBER_NAME_CHECKED(AOrbitDebugActor, Width)) && bAutoDraw)
		{
			DrawOrbits();
		}
	}
}
#endif
