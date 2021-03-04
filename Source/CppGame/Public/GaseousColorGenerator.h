// This is a copyright notice

#pragma once

#include "CoreMinimal.h"

class AActor;


/**
 * 
 */
class CPPGAME_API GaseousColorGenerator
{
public:
	GaseousColorGenerator(UObject* owner);
	~GaseousColorGenerator();

	UObject* Owner;

	UTexture2D* CreateTexture(FString TextureName, UCurveLinearColor* Gradient);

	UTexture2D* MakeVoronoiTexture(int16 NumStorms = 25, float StormFalloff = 2.5f, int LowBound = 100, int HighBound = 924, int TextureResolution = 1024);
};
