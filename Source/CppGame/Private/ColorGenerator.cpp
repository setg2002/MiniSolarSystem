// This is a copyright notice


#include "ColorGenerator.h"
#include "G:\UESource\Engine\Source\Runtime\Engine\Classes\Engine\TextureRenderTarget2D.h"
#include "G:\UESource\Engine\Source\Runtime\Engine\Classes\Curves\CurveLinearColor.h"
#include "G:\UESource\Engine\Source\Runtime\Engine\Classes\Curves\CurveLinearColorAtlas.h"
#include "G:\UESource\Engine\Source\Runtime\Engine\Classes\Materials\MaterialInstanceDynamic.h"

ColorGenerator::ColorGenerator(UColorSettings* colorSettings)
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

void ColorGenerator::UpdateColors()
{
	ColorSettings->Atlas->TextureSize = TextureResolution;
	ColorSettings->Atlas->GradientCurves.SetNum(TextureResolution);

	TArray<UCurveLinearColor*> Curves;
	Curves.Init(ColorSettings->Curve, TextureResolution);
	ColorSettings->Atlas->GradientCurves = Curves;
	ColorSettings->Atlas->UpdateTextures();

	for (int i = 0; i < ColorSettings->DynamicMaterials.Num(); i++)
	{
		ColorSettings->DynamicMaterials[i]->SetTextureParameterValue(FName("_texture"), Cast<UTexture>(ColorSettings->Atlas));
	}
}
