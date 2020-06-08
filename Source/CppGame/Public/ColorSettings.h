// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ColorSettings.generated.h"

/**
 * 
 */
UCLASS()
class CPPGAME_API UColorSettings : public UDataAsset
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere)
	FColor PlanetColor;


};
