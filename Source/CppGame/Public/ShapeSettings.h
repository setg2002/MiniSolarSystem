// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NoiseSettings.h"
#include "ShapeSettings.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew)
class CPPGAME_API UShapeSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//Radius in cm
	UPROPERTY(EditAnywhere)
	float PlanetRadius = 100;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	UNoiseSettings* noiseSettings;
};
