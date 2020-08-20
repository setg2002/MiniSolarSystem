// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ShapeGenerator.h"
#include "ColorGenerator.h"
#include "ColorSettings.h"

/**
 * 
 */

class CPPGAME_API TerrainFace
{
public:
	TerrainFace(ShapeGenerator* shapeGenerator, UProceduralMeshComponent* mesh, int resolution, FVector localUp);
	~TerrainFace();

	TArray<FVector> verticies;
	TArray<int> triangles;
	TArray<FVector2D> uv;
	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;

	UProceduralMeshComponent* Mesh;

	ShapeGenerator* shapeGenerator;

	TArray<FColor> VertexColors;

	int resolution;
	FVector localUp;
	FVector axisA;
	FVector axisB;

	void ColorMesh(UColorSettings* CS);
	void ConstructMesh();
	void UpdateUVs(ColorGenerator* colorGenerator);
	void UpdateTangentsNormals();
};
