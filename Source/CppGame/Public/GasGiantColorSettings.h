// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "SettingsAsset.h"
#include "GasGiantColorSettings.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FGasGiantColorSettings_
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite)
	UCurveLinearColor* Gradient = nullptr;

	UPROPERTY(SaveGame, EditAnywhere, Category = "Storm Settings", meta = (ClampMin = "0"))
	int NumStorms = 25;

	UPROPERTY(SaveGame, EditAnywhere, Category = "Storm Settings", meta = (ClampMin = "0"))
	float StormFalloff = 2.5f;
};

UCLASS()
class CPPGAME_API UGasGiantColorSettings : public USettingsAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(SaveGame, EditAnywhere)
	FGasGiantColorSettings_ ColorSettings;

public:
	UGasGiantColorSettings();

	FGasGiantColorSettings_ GetStruct() const { return ColorSettings; }
	bool SetStruct(FGasGiantColorSettings_ NewStruct)
	{
		ColorSettings = NewStruct;
		return true;
	}

	void SetMesh(UStaticMeshComponent* mesh) { Mesh = mesh; }

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BasePlanetMat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInstanceDynamic* DynamicMaterial;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateMaterial();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void NewVoronoiForStorms();

	UFUNCTION(BlueprintCallable)
	UCurveLinearColor* GetGradient() const { return ColorSettings.Gradient; }
	UFUNCTION(BlueprintCallable)
	void SetGradient(UCurveLinearColor* NewGradient);

	// Voronoi Settings \\

	UFUNCTION(BlueprintCallable)
	int GetNumStorms() const { return ColorSettings.NumStorms; }
	UFUNCTION(BlueprintCallable)
	void SetNumStorms(int NewNumStorms);
	
	UFUNCTION(BlueprintCallable)
	float GetStormFalloff() const { return ColorSettings.StormFalloff; }
	UFUNCTION(BlueprintCallable)
	void SetStormFalloff(float NewStormFalloff);

	//UPROPERTY(EditAnywhere, Category = "Storm Settings", AdvancedDisplay, meta = (ClampMin = "0", ClampMax = "1024"))
	const int LowBound = 100;

	//UPROPERTY(EditAnywhere, Category = "Storm Settings", AdvancedDisplay, meta = (ClampMin = "0", ClampMax = "1024"))
	const int HighBound = 924;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UStaticMeshComponent* Mesh;
};
