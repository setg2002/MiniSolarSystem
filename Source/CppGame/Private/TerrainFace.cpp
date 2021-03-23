// This is a copyright notice


#include "TerrainFace.h"
#include "Planet.h"
#include "AssetRegistryModule.h"

TerrainFace::TerrainFace(ShapeGenerator* shape_Generator, TerrestrialColorGenerator* color_Generator, int resolution, FVector localUp, UProceduralMeshComponent* procMesh, AActor* owner)
{
	Owner = owner;

	colorGenerator = color_Generator;
	shapeGenerator = shape_Generator;
	Resolution = resolution;
	LocalUp = localUp;
	ProcMesh = procMesh;

	axisA = FVector(this->LocalUp.Y, this->LocalUp.Z, this->LocalUp.X);
	axisB = FVector().CrossProduct(this->LocalUp, axisA);
}

TerrainFace::~TerrainFace()
{
}

void TerrainFace::ConstructMesh(TerrestrialColorGenerator* color_Generator)
{
	int triIndex = 0;

	uv.Empty();
	uv.SetNum(Resolution * Resolution);
	for (int y = 0; y < Resolution; y++)
	{
		for (int x = 0; x < Resolution; x++)
		{
			int i = x + y * Resolution;
			FVector2D percent = FVector2D(x, y) / (Resolution - 1);
			FVector pointOnUnitCube = -LocalUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
			FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
			float unscaledElevation = shapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
			verticies.EmplaceAt(i, pointOnUnitSphere * shapeGenerator->GetScaledElevation(unscaledElevation));
			uv[i].X = color_Generator->BiomePercentFromPoint(pointOnUnitSphere);
			uv[i].Y = unscaledElevation;

			if (x != Resolution - 1 && y != Resolution - 1)
			{
				triangles.Insert(i, triIndex);
				triangles.Insert(i + Resolution + 1, triIndex + 1);
				triangles.Insert(i + Resolution, triIndex + 2);

				triangles.Insert(i, triIndex + 3);
				triangles.Insert(i + 1, triIndex + 4);
				triangles.Insert(i + Resolution + 1, triIndex + 5);

				triIndex += 6;
			}
		}
	}
	ProcMesh->CreateMeshSection(0, verticies, triangles, normals, uv, VertexColors, tangents, false);
}

void TerrainFace::CalculateMesh()
{
	int triIndex = 0;

	uv.Empty();
	uv.SetNum(Resolution * Resolution);
	for (int y = 0; y < Resolution; y++)
	{
		for (int x = 0; x < Resolution; x++)
		{
			int i = x + y * Resolution;
			FVector2D percent = FVector2D(x, y) / (Resolution - 1);
			FVector pointOnUnitCube = -LocalUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
			FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
			float unscaledElevation = shapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
			verticies.EmplaceAt(i, pointOnUnitSphere * shapeGenerator->GetScaledElevation(unscaledElevation));
			uv[i].X = colorGenerator->BiomePercentFromPoint(pointOnUnitSphere);
			uv[i].Y = unscaledElevation;

			if (x != Resolution - 1 && y != Resolution - 1)
			{
				triangles.Insert(i, triIndex);
				triangles.Insert(i + Resolution + 1, triIndex + 1);
				triangles.Insert(i + Resolution, triIndex + 2);

				triangles.Insert(i, triIndex + 3);
				triangles.Insert(i + 1, triIndex + 4);
				triangles.Insert(i + Resolution + 1, triIndex + 5);

				triIndex += 6;
			}
		}
	}
	UpdateTangentsNormals();
}

void TerrainFace::ConstructMeshAsync(TerrestrialColorGenerator* color_Generator)
{
	(new FAutoDeleteAsyncTask<CalculateMeshAsyncTask>(*this, Resolution, LocalUp, axisA, axisB, verticies, triangles, uv, shapeGenerator, colorGenerator))->StartBackgroundTask();
}

void TerrainFace::UpdateTangentsNormals()
{
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(verticies, triangles, uv, normals, tangents);
	CreateMesh();
}

void TerrainFace::UpdateTangentsNormalsAsync()
{
	(new FAutoDeleteAsyncTask<CalculateTangentsAsyncTask>(*this, verticies, triangles, uv, normals, tangents))->StartBackgroundTask();
}

void TerrainFace::CreateMesh()
{
	ProcMesh->CreateMeshSection(0, verticies, triangles, normals, uv, VertexColors, tangents, false);
	Cast<APlanet>(Owner)->ConvertAndSetStaticMesh(ProcMesh);
}
