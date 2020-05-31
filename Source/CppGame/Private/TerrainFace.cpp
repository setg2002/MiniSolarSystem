// This is a copyright notice


#include "TerrainFace.h"

TerrainFace::TerrainFace(UProceduralMeshComponent* mesh, int resolution, FVector localUp)
{
	this->Mesh = mesh;
	this->resolution = resolution;
	this->localUp = localUp;

	axisA = FVector(this->localUp.Y, this->localUp.Z, this->localUp.X);
	axisB = FVector().CrossProduct(this->localUp, axisA);
}

TerrainFace::~TerrainFace()
{
}

void TerrainFace::ConstructMesh()
{
	TArray<FVector> verticies;
	TArray<int> triangles;

	int triIndex = 0;

	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			int i = x + y * resolution;
			FVector2D percent = FVector2D(x, y) / (resolution - 1);
			FVector pointOnUnitCube = -localUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
			FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
			verticies.Add(pointOnUnitSphere * 100);

			if (x != resolution - 1 && y != resolution - 1)
			{
				triangles.Insert(i, triIndex);
				triangles.Insert(i + resolution + 1, triIndex + 1);
				triangles.Insert(i + resolution, triIndex + 2);

				triangles.Insert(i, triIndex + 3);
				triangles.Insert(i + 1, triIndex + 4);
				triangles.Insert(i + resolution + 1, triIndex + 5);

				triIndex += 6;
			}
		}
	}
	Mesh->CreateMeshSection(0, verticies, triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}


