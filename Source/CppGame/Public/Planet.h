// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "TerrainFace.h"
#include "ColorSettings.h"
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
class ShapeGenerator;

UCLASS()
class CPPGAME_API APlanet : public ACelestialBody
{
	GENERATED_BODY()
	
public:
	APlanet();

	/*Mesh stuff*/
	UPROPERTY(Category = "Mesh", EditAnywhere, BlueprintReadWrite)
	TArray<UProceduralMeshComponent*> meshes;
	TerrainFace* terrainFaces[6];

	ShapeGenerator* shapeGenerator;

	UPROPERTY(Category = "Settings", EditAnywhere, Instanced, BlueprintReadWrite)
	UColorSettings* ColorSettings;
	UPROPERTY(Category = "Settings", EditAnywhere, Instanced, BlueprintReadWrite)
	UShapeSettings* ShapeSettings;

	UPROPERTY(Category = "Mesh", EditAnywhere, BlueprintReadWrite)
	UMaterial* PlanetMat;

	UPROPERTY(Category = "Mesh", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "2", ClampMax = "256"))
	int resolution = 16;

	void Initialize();

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
