// Copyright Soren Gilbertson


#include "GasGiantColorSettings.h"
#include "AssetRegistryModule.h"
#include "GaseousColorGenerator.h"
#include "GasGiant.h"
#include "Materials/MaterialInstanceDynamic.h"


UGasGiantColorSettings::UGasGiantColorSettings()
{
	BasePlanetMat = LoadObject<UMaterialInterface>(NULL, TEXT("UMaterialInterface'/Game/Materials/Instances/M_GasGiant_Inst.M_GasGiant_Inst'"), NULL, LOAD_None, NULL);
}

void UGasGiantColorSettings::GenerateMaterial()
{
	ensure(BasePlanetMat);
	DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BasePlanetMat);
	DynamicMaterial->SetTextureParameterValue("_Texture", GaseousColorGenerator::CreateTexture("GasTexture", ColorSettings.Gradient));

	//AssetCleaner::CleanDirectory(EDirectoryFilterType::DataAssets);
}

void UGasGiantColorSettings::NewVoronoiForStorms()
{
	if (!DynamicMaterial)
	{
		GenerateMaterial();
	}
	UTexture* NewTexture = GaseousColorGenerator::MakeVoronoiTexture(ColorSettings.NumStorms, ColorSettings.StormFalloff, LowBound, HighBound);
	DynamicMaterial->SetTextureParameterValue("_StormTexture", NewTexture);
}

void UGasGiantColorSettings::SetGradient(UCurveLinearColor* NewGradient)
{
	ColorSettings.Gradient = NewGradient;
	if (!DynamicMaterial)
	{
		GenerateMaterial();
	}
	DynamicMaterial->SetTextureParameterValue(FName("_Texture"), GaseousColorGenerator::CreateTexture("GasTexture", ColorSettings.Gradient));
}

void UGasGiantColorSettings::SetNumStorms(int NewNumStorms)
{
	ColorSettings.NumStorms = NewNumStorms;
	NewVoronoiForStorms();
}

void UGasGiantColorSettings::SetStormFalloff(float NewStormFalloff)
{
	ColorSettings.StormFalloff = NewStormFalloff;
	NewVoronoiForStorms();
}

#if WITH_EDITOR
void UGasGiantColorSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(UGasGiantColorSettings, BasePlanetMat))
		{
			if (Mesh)
			{
				GenerateMaterial();
			}
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FGasGiantColorSettings_, Gradient) && ColorSettings.Gradient != nullptr)
		{
			if (Mesh)
			{
				GenerateMaterial();
				DynamicMaterial->SetTextureParameterValue(FName("_Texture"), GaseousColorGenerator::CreateTexture("GasTexture", ColorSettings.Gradient));
			}
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
