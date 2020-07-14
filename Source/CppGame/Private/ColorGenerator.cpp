// This is a copyright notice


#include "ColorGenerator.h"
#include "G:\UESource\Engine\Source\Runtime\Engine\Classes\Curves\CurveLinearColor.h"
#include "G:\UESource\Engine\Source\Runtime\Engine\Classes\Curves\CurveLinearColorAtlas.h"
#include "G:\UESource\Engine\Source\Runtime\Engine\Classes\Materials\MaterialInstanceDynamic.h"

void ColorGenerator::UpdateSettings(UColorSettings* colorSettings)
{
	this->ColorSettings = colorSettings;
}

ColorGenerator::~ColorGenerator()
{
}

void ColorGenerator::UpdateElevation(MinMax* elevationMinMax)
{
	for (int i = 0; i < ColorSettings->DynamicMaterials.Num(); i++)
	{
		ColorSettings->DynamicMaterials[i]->SetVectorParameterValue(FName("_elevationMinMax"), FLinearColor(elevationMinMax->Min, elevationMinMax->Max, 0, 0));
	}
}

float ColorGenerator::BiomePercentFromPoint(FVector PointOnUnitSphere)
{
	float HeightPercent = (PointOnUnitSphere.Y + 1) / 2.f;
	float BiomeIndex = 0;
	int NumBiomes = ColorSettings->BiomeColorSettings->Biomes.Num();

	for (int i = 0; i < NumBiomes; i++)
	{
		if (ColorSettings->BiomeColorSettings->Biomes[i]->StartHeight < HeightPercent)
		{
			BiomeIndex = i;
		}
		else { break; }
	}
	return BiomeIndex / FMath::Max<float>(1, NumBiomes - 1);
}

void ColorGenerator::UpdateColors()
{
	ColorSettings->Atlas->TextureSize = TextureResolution;
	ColorSettings->Atlas->GradientCurves.SetNum(ColorSettings->BiomeColorSettings->Biomes.Num());
	TArray<UCurveLinearColor*> BaseCurves;
	for (int i = 0; i < ColorSettings->BiomeColorSettings->Biomes.Num(); i++)
	{
		for (int k = 0; k < TextureResolution / ColorSettings->BiomeColorSettings->Biomes.Num(); k++)
		{
			BaseCurves.Add(ColorSettings->BiomeColorSettings->Biomes[i]->Gradient);
		}
	}
	// This is stupid, but without it the loops above don't add the full number of gradients
	BaseCurves.Add(ColorSettings->BiomeColorSettings->Biomes.Last(0)->Gradient);

	ColorSettings->Atlas->GradientCurves = BaseCurves;
	ColorSettings->Atlas->UpdateTextures();

	ColorSettings->BiomeTintAtlas->TextureSize = ColorSettings->BiomeColorSettings->Biomes.Num();
	ColorSettings->BiomeTintAtlas->GradientCurves.SetNum(ColorSettings->BiomeColorSettings->Biomes.Num());
	TArray<UCurveLinearColor*> TintCurves;
	for (auto& biome : ColorSettings->BiomeColorSettings->Biomes)
	{
		auto Tint = NewObject<UCurveLinearColor>();
		auto Keys = TArray<FRichCurveKey>();
		for (int i = 0; i < 3; i++)
		{
			switch (i)
			{
			case 0:
				Keys.Add(FRichCurveKey(0, biome->Tint.R));
				Keys.Add(FRichCurveKey(1, biome->Tint.R));
			case 1:
				Keys.Add(FRichCurveKey(0, biome->Tint.G));
				Keys.Add(FRichCurveKey(1, biome->Tint.G));
			case 2:
				Keys.Add(FRichCurveKey(0, biome->Tint.B));
				Keys.Add(FRichCurveKey(1, biome->Tint.B));
			default:
				break;
			}
			Tint->FloatCurves[i].SetKeys(Keys);
		}
		TintCurves.Add(Tint);
	}
	ColorSettings->BiomeTintAtlas->GradientCurves.Empty();
	ColorSettings->BiomeTintAtlas->GradientCurves = TintCurves;
	ColorSettings->BiomeTintAtlas->UpdateTextures();

	for (int i = 0; i < ColorSettings->DynamicMaterials.Num(); i++)
	{
		ColorSettings->DynamicMaterials[i]->SetTextureParameterValue(FName("_texture"), Cast<UTexture>(ColorSettings->Atlas));
		ColorSettings->DynamicMaterials[i]->SetTextureParameterValue(FName("_biomeTint"), Cast<UTexture>(ColorSettings->BiomeTintAtlas));
		ColorSettings->DynamicMaterials[i]->SetScalarParameterValue(FName("_tintPercent"), ColorSettings->BiomeTintPercent);
	}
}
