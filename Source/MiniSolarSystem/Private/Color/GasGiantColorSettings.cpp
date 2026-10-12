// Copyright Soren Gilbertson


#include "Color/GasGiantColorSettings.h"

#include "Color/ColorCurveFunctionLibrary.h"

void FGasGiantColorSettings::Randomize()
{
	NumStorms = FMath::RandRange(0, 30);
	StormFalloff = FMath::RandRange(0.0f, 15.0f);
		
	int32 NumberOfPoints = FMath::RandRange(2, 50);
	int8 MaxHueDelta = FMath::RandRange(1, 20);
	Gradient = UColorCurveFunctionLibrary::CreateRandomCurve(NumberOfPoints, true, MaxHueDelta);
}
