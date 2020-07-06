// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NoiseSettings.h"
#include "NoiseLayer.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew)
class CPPGAME_API UNoiseLayer : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool Enabled = true;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool UseFirstLayerAsMask = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Instanced)
	UNoiseSettings* NoiseSettings;
	
};
