// Copyright Soren Gilbertson

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

	DECLARE_MULTICAST_DELEGATE(FNoiseLayerChanged);

private:
	UPROPERTY(EditAnywhere)
	bool Enabled = true;
	UPROPERTY(EditAnywhere)
	bool UseFirstLayerAsMask = false;

public:
	FNoiseLayerChanged OnNoiseLayerChanged;

	UFUNCTION(BlueprintCallable)
	bool GetEnabled() const { return Enabled; }
	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool NewEnabled);

	UFUNCTION(BlueprintCallable)
	bool GetFirstLayerAsMask() const { return UseFirstLayerAsMask; }
	UFUNCTION(BlueprintCallable)
	void SetFirstLayerAsMask(bool NewFirstLayerAsMask);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNoiseSettings* NoiseSettings;
	
};
