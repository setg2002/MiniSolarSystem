// Copyright Soren Gilbertson


#include "GasGiantColorSettings.h"
#include "AssetRegistryModule.h"
#include "GaseousColorGenerator.h"
#include "GasGiant.h"
#include "Materials/MaterialInstanceDynamic.h"


UGasGiantColorSettings::UGasGiantColorSettings()
{
}


void UGasGiantColorSettings::SetOwner(AGasGiant* owner)
{
	Owner = owner;
}

#if WITH_EDITOR
void UGasGiantColorSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(UGasGiantColorSettings, BasePlanetMat))
		{
			if (Owner)
			{
				Owner->GenerateMaterial();
			}
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(UGasGiantColorSettings, Gradient) && Gradient != nullptr)
		{
			if (Owner)
			{
				Owner->GenerateMaterial();
				DynamicMaterial->SetTextureParameterValue(FName("_Texture"), Owner->ColorGenerator->CreateTexture("GasTexture", Gradient));
			}
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
