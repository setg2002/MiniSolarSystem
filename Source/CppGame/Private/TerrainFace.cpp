// This is a copyright notice


#include "TerrainFace.h"
#include "KismetProceduralMeshLibrary.h"

TerrainFace::TerrainFace(ShapeGenerator* shapeGenerator, UProceduralMeshComponent* mesh, int resolution, FVector localUp)
{
	this->shapeGenerator = shapeGenerator;
	this->Mesh = mesh;
	this->resolution = resolution;
	this->localUp = localUp;

	axisA = FVector(this->localUp.Y, this->localUp.Z, this->localUp.X);
	axisB = FVector().CrossProduct(this->localUp, axisA);
}

TerrainFace::~TerrainFace()
{
}

void TerrainFace::ColorMesh(UColorSettings* CS)
{
	/*for (int j = 0; j < verticies.Num(); j++)
	{
		//VertexColors.Add(CS->PlanetColor);
	}*/

	//Mesh->UpdateMeshSection(0, verticies, normals, uv, VertexColors, tangents);
}

void TerrainFace::ConstructMesh()
{
	int triIndex = 0;
	uv.SetNum(resolution * resolution);

	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			int i = x + y * resolution;
			FVector2D percent = FVector2D(x, y) / (resolution - 1);
			FVector pointOnUnitCube = -localUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
			FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
			float unscaledElevation = shapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
			verticies.EmplaceAt(i, pointOnUnitSphere * shapeGenerator->GetScaledElevation(unscaledElevation));
			uv[i].Y = unscaledElevation;

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

	Mesh->CreateMeshSection(0, verticies, triangles, normals, uv, VertexColors, tangents, false);
}

void TerrainFace::UpdateUVs(ColorGenerator* colorGenerator)
{
	uv.SetNum(resolution * resolution);
	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			int i = x + y * resolution;
			FVector2D percent = FVector2D(x, y) / (resolution - 1);
			FVector pointOnUnitCube = -localUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
			FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
			//UE_LOG(LogTemp, Warning, TEXT("pointOnUnitSphere: %s, BiomePercent: %f"), *pointOnUnitSphere.ToString(), colorGenerator->BiomePercentFromPoint(pointOnUnitSphere));
			uv[i].X = colorGenerator->BiomePercentFromPoint(pointOnUnitSphere);
		}
	}

	Mesh->UpdateMeshSection(0, verticies, normals, uv, VertexColors, tangents);
}

void TerrainFace::UpdateTangentsNormals()
{
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(verticies, triangles, TArray<FVector2D>(), normals, tangents);
	Mesh->UpdateMeshSection(0, verticies, normals, uv, VertexColors, tangents);
}
