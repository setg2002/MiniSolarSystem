// This is a copyright notice


#include "Star.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/DirectionalLightComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"


AStar::AStar()
{
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(FName("Sphere"));
	Sphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Sphere->CastShadow = 0;
	Sphere->bCastDynamicShadow = 0;

	this->initialVelocity = FVector::ZeroVector;

	dynamicMaterial = Sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, Sphere->GetMaterial(0));
	Sphere->SetMaterial(0, dynamicMaterial);

	Light = CreateDefaultSubobject<UDirectionalLightComponent>("Light");

	SolarParticleTemplate = LoadObject<UNiagaraSystem>(NULL, TEXT("NiagaraSystem'/Game/Particles/Star/SolarNiagaraSystem.SolarNiagaraSystem'"), NULL, LOAD_None, NULL);
}

void AStar::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	//TODO The system should be attached to the star
	//TODO Sometimes multiple instances are created (possibly ReinitializeSystem() fault?), eating up lots of memory

	//ParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(SolarParticleTemplate, Sphere, FName(""), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);	
	ParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SolarParticleTemplate, Sphere->GetComponentLocation());
#if !UE_BUILD_DEVELOPMENT //FIX Without this the game will not package for some reason
	ParticleComponent->SetNiagaraVariableLinearColor(FString("User.StarColor"), starProperties.color);
	ParticleComponent->SetNiagaraVariableFloat(FString("User.Radius"), float(starProperties.radius) * 100.f);
	ParticleComponent->ReinitializeSystem();
	ParticleComponent->Activate();
#endif
	//UE_LOG(LogTemp, Warning, TEXT("%s"), ParticleComponent != nullptr ? TEXT("True") : TEXT("False"));
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *ParticleComponent->GetAttachParent()->GetName());
}

void AStar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Light->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()));
}

void AStar::ReInitParticles()
{
	if (ParticleComponent) { ParticleComponent->Deactivate(); ParticleComponent->DestroyComponent(); }
	//ParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(SolarParticleTemplate, Sphere, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	ParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SolarParticleTemplate, Sphere->GetComponentLocation());
	ParticleComponent->SetNiagaraVariableLinearColor(FString("User.StarColor"), starProperties.color);
	ParticleComponent->SetNiagaraVariableFloat(FString("User.Radius"), float(starProperties.radius) * 100.f);
	ParticleComponent->ReinitializeSystem();
}

void AStar::UpdateColor()
{
	if (Sphere->GetMaterial(0) != dynamicMaterial)
	{
		dynamicMaterial = Sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, Sphere->GetMaterial(0));
		Sphere->SetMaterial(0, dynamicMaterial);
	}
	dynamicMaterial->SetVectorParameterValue(FName("_baseColor"), starProperties.color);
	ParticleComponent->SetNiagaraVariableLinearColor(FString("User.StarColor"), starProperties.color);
	ParticleComponent->ReinitializeSystem();
}

#if WITH_EDITOR
void AStar::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(FStarProperties, radius))
		{
			Sphere->SetRelativeScale3D(FVector(starProperties.radius, starProperties.radius, starProperties.radius));
			ParticleComponent->SetNiagaraVariableFloat(FString("User.Radius"), float(starProperties.radius) * 100.f);
			ParticleComponent->ReinitializeSystem();
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FStarProperties, mass))
		{
			this->mass = starProperties.mass;
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FStarProperties, color))
		{
			if (Sphere->GetMaterial(0) != dynamicMaterial)
			{
				dynamicMaterial = Sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, Sphere->GetMaterial(0));
				Sphere->SetMaterial(0, dynamicMaterial);
			}
			if (planetMateralParameterCollectionInst == nullptr)
			{
				planetMateralParameterCollectionInst = GetWorld()->GetParameterCollectionInstance(planetMateralParameterCollection);
			}
			UpdateColor();
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FStarProperties, luminosity))
		{
			if (Sphere->GetMaterial(0) != dynamicMaterial)
			{
				dynamicMaterial = Sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, Sphere->GetMaterial(0));
				Sphere->SetMaterial(0, dynamicMaterial);
			}
			if (planetMateralParameterCollectionInst == nullptr)
			{
				planetMateralParameterCollectionInst = GetWorld()->GetParameterCollectionInstance(planetMateralParameterCollection);
			}
			dynamicMaterial->SetScalarParameterValue(FName("_glowPower"), starProperties.luminosity);
			planetMateralParameterCollectionInst->SetScalarParameterValue(FName("StarLuminosity"), starProperties.luminosity);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(AStar, starType))
		{
			static const FString ContextString(TEXT("Star Type"));
			starProperties = *(starTypeData->FindRow<FStarProperties>(FName(UEnum::GetValueAsString<EStarType>(starType.GetValue())), ContextString, true));
			Sphere->SetRelativeScale3D(FVector(starProperties.radius, starProperties.radius, starProperties.radius));
			this->mass = starProperties.mass;

			if (Sphere->GetMaterial(0) != dynamicMaterial)
			{
				dynamicMaterial = Sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, Sphere->GetMaterial(0));
				Sphere->SetMaterial(0, dynamicMaterial);
			}
			dynamicMaterial->SetVectorParameterValue(FName("_baseColor"), starProperties.color);
			dynamicMaterial->SetScalarParameterValue(FName("_glowPower"), starProperties.luminosity);
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void AStar::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (planetMateralParameterCollectionInst == nullptr)
	{
		planetMateralParameterCollectionInst = GetWorld()->GetParameterCollectionInstance(planetMateralParameterCollection);
	}
	planetMateralParameterCollectionInst->SetVectorParameterValue(FName("SunLocation"), this->GetActorLocation());
}
#endif
