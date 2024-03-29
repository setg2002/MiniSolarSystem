// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"

class AActor;


/**
 * 
 */
class MINISOLARSYSTEM_API GaseousColorGenerator
{
public:
	GaseousColorGenerator();
	~GaseousColorGenerator();

	static UTexture2D* CreateTexture(FString TextureName, UCurveLinearColor* Gradient);

	static UTexture2D* MakeVoronoiTexture(int16 NumStorms = 25, float StormFalloff = 2.5f, int LowBound = 100, int HighBound = 924, int TextureResolution = 1024);
};
