// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ColorSettings.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew)
class CPPGAME_API UColorSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UColorSettings();

	UPROPERTY(EditAnywhere)
	FColor PlanetColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* PlanetMat;

	TArray<UMaterialInstanceDynamic*> DynamicMaterials;
};
