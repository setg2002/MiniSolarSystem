// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "GasGiant.generated.h"

/**
 * 
 */

// Forward Declarations
class UGasGiantColorSettings;


UCLASS()
class CPPGAME_API AGasGiant : public ACelestialBody
{
	GENERATED_BODY()
	
public:
	AGasGiant();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGasGiantColorSettings* ColorSettings;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateMaterial();

	UTexture2D* CreateTexture(FString TextureName, UCurveLinearColor* Gradient);

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
