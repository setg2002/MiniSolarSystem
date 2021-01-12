// This is a copyright notice


#include "ColorSettings.h"

UColorSettings::UColorSettings()
{
	DynamicMaterials.SetNum(6);
	BiomeColorSettings = (UBiomeColorSettings*)StaticConstructObject_Internal(BiomeColorSettings->StaticClass());
	BiomeColorSettings->Biomes.SetNum(1);
}
