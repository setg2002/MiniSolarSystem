// This is a copyright notice


#include "GasGiant.h"
#include "AssetCleaner.h"
#include "GasGiantColorSettings.h"
#include "GaseousColorGenerator.h"
#include "Materials/MaterialInstanceDynamic.h"


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

	AssetCleaner::CleanDirectory(EDirectoryFilterType::DataAssets);
}

void AGasGiant::NewVoronoiForStorms()
{
	//GenerateMaterial();
#if WITH_EDITOR
	ColorSettings->DynamicMaterial->SetTextureParameterValue(FName("_StormTexture"), ColorGenerator->MakeVoronoiTexture(ColorSettings->NumStorms, ColorSettings->StormFalloff, ColorSettings->LowBound, ColorSettings->HighBound));
#endif
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
