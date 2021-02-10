// This is a copyright notice


#include "GasGiant.h"
#include "GasGiantColorSettings.h"


AGasGiant::AGasGiant()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
}

void AGasGiant::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(UGasGiantColorSettings, BasePlanetMat))
		{
			ColorSettings->DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, ColorSettings->BasePlanetMat);
		}
	}
}
