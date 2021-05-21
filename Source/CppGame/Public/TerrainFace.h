// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

struct FProcMeshTangent;

class TerrestrialColorGenerator;
class UProceduralMeshComponent;
class ShapeGenerator;
class AActor;


class CPPGAME_API TerrainFace
{
public:
	TerrainFace(int i, ShapeGenerator* shape_Generator, TerrestrialColorGenerator* color_Generator, int resolution, FVector localUp, UProceduralMeshComponent* mesh, AActor* owner);
	~TerrainFace();

	AActor* Owner;

	TArray<FVector> verticies;
	TArray<int> triangles;
	TArray<FVector2D> uv;
	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;

	UProceduralMeshComponent* ProcMesh;

	TerrestrialColorGenerator* colorGenerator;
	ShapeGenerator* shapeGenerator;

	TArray<FColor> VertexColors;

	int MeshSection;
	int Resolution;
	FVector LocalUp;
	FVector axisA;
	FVector axisB;

	void ConstructMesh(TerrestrialColorGenerator* colorGenerator);
	void ConstructMeshAsync(TerrestrialColorGenerator* colorGenerator);

	void CalculateMesh();

	void UpdateBiomePercents();

	void CreateMesh();
	void UpdateTangentsNormals();
	void UpdateTangentsNormalsAsync();

private:
	TArray<FVector> PointsOnUnitSphere;
};
