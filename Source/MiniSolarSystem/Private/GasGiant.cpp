// Copyright Soren Gilbertson


#include "GasGiant.h"
#include "OrbitDebugActor.h"
#include "CelestialGameMode.h"
#include "GasGiantColorSettings.h"
#include "GaseousColorGenerator.h"
#include "Materials/MaterialInstanceDynamic.h"


AGasGiant::AGasGiant()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Collider->SetSphereRadius(102);
	ColorGenerator = new GaseousColorGenerator();
}

void AGasGiant::BeginPlay()
{
	Super::BeginPlay();
	
	this->SetActorScale3D(FVector(Radius));
	ColorSettings->SetMesh(Mesh);
	ColorSettings->GenerateMaterial();
	ColorSettings->NewVoronoiForStorms();
}

void AGasGiant::SetRadius(int NewRadius)
{
	Radius = NewRadius;
	this->SetActorScale3D(FVector(Radius));

	if (Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentPerspective() == 0)
		AOrbitDebugActor::Get()->UpdateWidthSpecificBody(this);
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
