// This is a copyright notice


#include "GasGiant.h"
#include "GasGiantColorSettings.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GaseousColorGenerator.h"


AGasGiant::AGasGiant()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
	ColorGenerator = new GaseousColorGenerator(this);
}

void AGasGiant::GenerateMaterial()
{
	ensure(ColorSettings->BasePlanetMat);
	ColorSettings->SetOwner(this);
	ColorSettings->DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, ColorSettings->BasePlanetMat);
}

void AGasGiant::NewVoronoiForStorms()
{
	GenerateMaterial();
	ColorSettings->DynamicMaterial->SetTextureParameterValue(FName("_StormTexture"), ColorGenerator->MakeVoronoiTexture(ColorSettings->NumStorms, ColorSettings->StormFalloff, ColorSettings->LowBound, ColorSettings->HighBound));
}


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
