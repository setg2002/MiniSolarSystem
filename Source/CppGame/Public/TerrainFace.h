// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

/**
 * 
 */
class CPPGAME_API TerrainFace
{
public:
	TerrainFace(UProceduralMeshComponent* mesh, int resolution, FVector localUp);
	~TerrainFace();


	UProceduralMeshComponent* Mesh;

	int resolution;
	FVector localUp;
	FVector axisA;
	FVector axisB;

	void ConstructMesh();

};
