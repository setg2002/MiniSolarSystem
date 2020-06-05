// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "TerrainFace.h"
#include "Planet.generated.h"

/**
 * 
 */
UCLASS()
class CPPGAME_API APlanet : public ACelestialBody
{
	GENERATED_BODY()
	
public:
	APlanet();

	/*Mesh stuff*/
	TArray<UProceduralMeshComponent*> meshes;
	TerrainFace* terrainFaces[6];

	UPROPERTY(Category = "Mesh", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "2", ClampMax = "256"))
	int resolution = 16;

	void Initialize();

	void GenerateMesh();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
