// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "TerrainFace.h"
#include "ColorSettings.h"
#include "ColorGenerator.h"
#include "ShapeSettings.h"
#include "ShapeGenerator.h"
#include "Planet.generated.h"

/**
 * 
 */

//Forward Declarations
class TerrainFace;
class UColorSettings;
class UShapeSettings;

UENUM()
enum EFaceRenderMask { 
	All      UMETA(DisplayName = "All Faces"),
	Bottom   UMETA(DisplayName = "Bottom"),
	Top      UMETA(DisplayName = "Top"),
	Left     UMETA(DisplayName = "Left"),
	Right    UMETA(DisplayName = "Right"),
	Back     UMETA(DisplayName = "Back"),
	Front    UMETA(DisplayName = "Front")
};


UCLASS()
class CPPGAME_API APlanet : public ACelestialBody
{
	GENERATED_BODY()
	
public:
	APlanet();

	UPROPERTY(Category = "Settings", EditAnywhere)
	bool AutoGenerate;

	/*Mesh stuff*/
	UPROPERTY(Category = "Mesh", EditAnywhere, BlueprintReadWrite)
	TArray<UProceduralMeshComponent*> meshes;
	TerrainFace* terrainFaces[6];

	ShapeGenerator* shapeGenerator;
	ColorGenerator* colorGenerator;

	UPROPERTY(Category = "Settings", EditInstanceOnly)
	TEnumAsByte<EFaceRenderMask> FaceRenderMask;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	UColorSettings* ColorSettings;
	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	UShapeSettings* ShapeSettings;

	UPROPERTY(Category = "Mesh", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "2", ClampMax = "256"))
	int resolution = 16;

	void Initialize();

	UFUNCTION(Category = "Settings", BlueprintCallable, CallInEditor)
	void ReGenerate();

	UFUNCTION(Category = "Settings", BlueprintCallable, CallInEditor)
	void ReGenerateColors();

	UFUNCTION(Category = "Settings", BlueprintCallable, CallInEditor)
	void ReGenerateTangents();

	void GeneratePlanet();

	void GenerateMesh();

	void GenerateColors();

	void OnShapeSettingsUpdated();

	void OnColorSettingsUpdated();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

protected:

	const FVector directions[6] = { FVector().UpVector,
									FVector().DownVector,
									FVector().LeftVector,
									FVector().RightVector,
									FVector().ForwardVector,
									FVector().BackwardVector };
};
