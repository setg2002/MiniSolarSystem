// Copyright Soren Gilbertson


#include "TerrainFace.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "ShapeGenerator.h"
#include "ColorGenerator.h"
#include "ColorSettings.h"
#include "Planet.h"


TerrainFace::TerrainFace(int i, ShapeGenerator* shape_Generator, TerrestrialColorGenerator* color_Generator, int resolution, FVector localUp, UProceduralMeshComponent* procMesh, AActor* owner)
{
	Owner = owner;

	colorGenerator = color_Generator;
	shapeGenerator = shape_Generator;
	Resolution = resolution;
	LocalUp = localUp;
	ProcMesh = procMesh;
	MeshSection = i;

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
	ProcMesh->CreateMeshSection(MeshSection, verticies, triangles, normals, uv, VertexColors, tangents, true);
}

void TerrainFace::CalculateMesh()
{
	int triIndex = 0;

	PointsOnUnitSphere.Empty();
	PointsOnUnitSphere.SetNum(Resolution * Resolution);

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
			PointsOnUnitSphere[i] = pointOnUnitSphere;
			float unscaledElevation = shapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
			float scaledElevation = shapeGenerator->GetScaledElevation(unscaledElevation);
			verticies.EmplaceAt(i, pointOnUnitSphere * scaledElevation);
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

void TerrainFace::UpdateBiomePercents()
{
	for (int y = 0; y < Resolution; y++)
	{
		for (int x = 0; x < Resolution; x++)
		{
			int i = x + y * Resolution;

			uv[i].X = colorGenerator->BiomePercentFromPoint(PointsOnUnitSphere[i]);
		}
	}
	ProcMesh->CreateMeshSection(MeshSection, verticies, triangles, normals, uv, VertexColors, tangents, false);
}

void TerrainFace::ConstructMeshAsync(TerrestrialColorGenerator* color_Generator)
{
	AsyncTask(ENamedThreads::AnyThread, [this]() { CalculateMesh(); });
}

void TerrainFace::UpdateTangentsNormals()
{
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(verticies, triangles, uv, normals, tangents);
	AsyncTask(ENamedThreads::GameThread, [this]() { CreateMesh(); }); 
}

void TerrainFace::UpdateTangentsNormalsAsync()
{
	AsyncTask(ENamedThreads::AnyThread, [this]() { UpdateTangentsNormals(); });
}

void TerrainFace::CreateMesh()
{
	ProcMesh->CreateMeshSection(MeshSection, verticies, triangles, normals, uv, VertexColors, tangents, false);
	Cast<APlanet>(Owner)->ReconveneTerrainFaceThreads(MeshSection);
}
