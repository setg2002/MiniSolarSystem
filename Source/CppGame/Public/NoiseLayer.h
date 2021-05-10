// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NoiseSettings.h"
#include "NoiseLayer.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CPPGAME_API UNoiseLayer : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Enabled = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseFirstLayerAsMask = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNoiseSettings* NoiseSettings;
	
};
