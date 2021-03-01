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

	UTexture2D* MakeVoronoiTexture(int8 NumStorms, float StormFalloff, int LowBound, int HighBound);
};
