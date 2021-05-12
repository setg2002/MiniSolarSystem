// Copyright Soren Gilbertson


#include "ColorSettings.h"

UColorSettings::UColorSettings()
{
	PlanetMat = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/Instances/M_Planet_Inst.M_Planet_Inst'"), NULL, LOAD_None, NULL);
}
