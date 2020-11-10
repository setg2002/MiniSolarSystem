// This is a copyright notice


#include "TerrainFace.h"
#include "G:\UESource\Engine\Source\Runtime\Core\Public\HAL\IConsoleManager.h"

TerrainFace::TerrainFace(ShapeGenerator* shapeGenerator, ColorGenerator* color_Generator, UProceduralMeshComponent* mesh, int resolution, FVector localUp)
{
	this->colorGenerator = color_Generator;
	this->shapeGenerator = shapeGenerator;
	this->resolution = resolution;
	this->localUp = localUp;
	Mesh = mesh;

	axisA = FVector(this->localUp.Y, this->localUp.Z, this->localUp.X);
	axisB = FVector().CrossProduct(this->localUp, axisA);

	MeshDelegate.BindRaw(this, &TerrainFace::CreateMesh);
}

TerrainFace::~TerrainFace()
{
}

void TerrainFace::ConstructMesh(ColorGenerator* color_Generator)
{
	int triIndex = 0;

	uv.Empty();
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
			uv[i].X = color_Generator->BiomePercentFromPoint(pointOnUnitSphere);
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

void TerrainFace::CalculateMesh()
{
	int triIndex = 0;

	uv.Empty();
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
			uv[i].X = colorGenerator->BiomePercentFromPoint(pointOnUnitSphere);
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
	return;
}

void TerrainFace::ConstructMeshAsync(ColorGenerator* color_Generator)
{
	TArray<FVector>& vertsRef = verticies;
	TArray<int>& trisRef = triangles;
	TArray<FVector2D>& uvRef = uv;
	
	(new FAutoDeleteAsyncTask<CalculateMeshAsyncTask>(*this, resolution, localUp, axisA, axisB, vertsRef, trisRef, uvRef, shapeGenerator, colorGenerator))->StartBackgroundTask();
}

void TerrainFace::CreateMesh()
{
	Mesh->CreateMeshSection(0, verticies, triangles, normals, uv, VertexColors, tangents, false);
}

void TerrainFace::UpdateTangentsNormals()
{
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(verticies, triangles, uv, normals, tangents);
	CreateMesh();
}

void TerrainFace::UpdateTangentsNormalsAsync()
{
	TArray<FVector>& vertsRef = verticies;
	TArray<int>& trisRef = triangles;
	TArray<FVector2D>& uvRef = uv;
	TArray<FVector>& normalsRef = normals;
	TArray<FProcMeshTangent>& tangentsRef = tangents;

	(new FAutoDeleteAsyncTask<CalculateTangentsAsyncTask>(*this, vertsRef, trisRef, uvRef, normalsRef, tangentsRef))->StartBackgroundTask();
}
