// This is a copyright notice


#include "ColorSettings.h"

UColorSettings::UColorSettings()
{
	PlanetMat = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/Instances/PlanetMat_Inst.PlanetMat_Inst'"), NULL, LOAD_None, NULL);
	DynamicMaterials.SetNum(6);
}
