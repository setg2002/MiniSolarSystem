// Copyright Soren Gilbertson


#include "GasGiant.h"
#include "OrbitDebugActor.h"
#include "CelestialGameMode.h"
#include "GaseousColorGenerator.h"
#include "Materials/MaterialInstanceDynamic.h"


AGasGiant::AGasGiant()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Collider->SetSphereRadius(102);
	ColorGenerator = new GaseousColorGenerator();
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Game/Materials/Instances/M_GasGiant_Inst.M_GasGiant_Inst"));
	BasePlanetMat = Material.Object;
}

void AGasGiant::BeginPlay()
{
	Super::BeginPlay();
	
	this->SetActorScale3D(FVector(Radius));
	GenerateMaterial();
	NewVoronoiForStorms();
}

void AGasGiant::ReInit()
{
	this->SetActorScale3D(FVector(Radius));
	GenerateMaterial();
	NewVoronoiForStorms();
}

void AGasGiant::SetRadius(float NewRadius)
{
	Radius = NewRadius;
	this->SetActorScale3D(FVector(Radius));

	if (Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentPerspective() == 0)
		AOrbitDebugActor::Get()->UpdateWidthSpecificBody(this);
}

void AGasGiant::GenerateMaterial()
{
	ensure(BasePlanetMat);
	DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BasePlanetMat);
	if (ColorSettings.Gradient)
		DynamicMaterial->SetTextureParameterValue("_Texture", GaseousColorGenerator::CreateTexture("GasTexture", ColorSettings.Gradient));
}

void AGasGiant::NewVoronoiForStorms()
{
	if (!DynamicMaterial)
		GenerateMaterial();
	UTexture* NewTexture = GaseousColorGenerator::MakeVoronoiTexture(ColorSettings.NumStorms, ColorSettings.StormFalloff, LowBound, HighBound);
	DynamicMaterial->SetTextureParameterValue("_StormTexture", NewTexture);
}

void AGasGiant::SetGradient(UCurveLinearColor* NewGradient)
{
	ColorSettings.Gradient = NewGradient;
	if (!DynamicMaterial)
		GenerateMaterial();
	if (ColorSettings.Gradient)
		DynamicMaterial->SetTextureParameterValue(FName("_Texture"), GaseousColorGenerator::CreateTexture("GasTexture", ColorSettings.Gradient));
}

void AGasGiant::SetNumStorms(int NewNumStorms)
{
	ColorSettings.NumStorms = NewNumStorms;
	NewVoronoiForStorms();
}

void AGasGiant::SetStormFalloff(float NewStormFalloff)
{
	ColorSettings.StormFalloff = NewStormFalloff;
	NewVoronoiForStorms();
}

#if WITH_EDITOR
void AGasGiant::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(AGasGiant, Radius))
		{
			this->SetActorScale3D(FVector(Radius));
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
