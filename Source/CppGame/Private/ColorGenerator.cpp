// This is a copyright notice


#include "ColorGenerator.h"
#include "E:\UESource\Engine\Source\Runtime\Engine\Classes\Materials\MaterialInstanceDynamic.h"

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
		UE_LOG(LogTemp, Warning, TEXT("Min: %f, Max: %f"), elevationMinMax->Min, elevationMinMax->Max);
	}
}
