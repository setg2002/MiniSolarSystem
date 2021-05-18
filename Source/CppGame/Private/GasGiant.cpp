// Copyright Soren Gilbertson


#include "GasGiant.h"
#include "GasGiantColorSettings.h"
#include "GaseousColorGenerator.h"
#include "Materials/MaterialInstanceDynamic.h"


AGasGiant::AGasGiant()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
	ColorGenerator = new GaseousColorGenerator();
}

void AGasGiant::BeginPlay()
{
	Super::BeginPlay();
	
	ColorSettings->SetMesh(Mesh);
}

void AGasGiant::SetRadius(int NewRadius)
{
	Radius = NewRadius;
	this->SetActorScale3D(FVector(Radius));
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
