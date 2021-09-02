// Copyright Soren Gilbertson


#include "TerrainFace.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "ShapeGenerator.h"
#include "ColorGenerator.h"
#include "ColorSettings.h"
#include "Planet.h"


TerrainFace::TerrainFace(int32 i, ShapeGenerator* shape_Generator, TerrestrialColorGenerator* color_Generator, int32 resolution, FVector localUp, UProceduralMeshComponent* procMesh, AActor* owner)
	: ProcMesh(procMesh), colorGenerator(color_Generator), shapeGenerator(shape_Generator), MeshSection(i)
{
	Owner = owner;

	Data = FTerrainFaceData(resolution, localUp);
}

TerrainFace::~TerrainFace()
{
	if (Worker)
	{
		Worker->EnsureCompletion();
	}
}

void TerrainFace::ConstructMesh(TerrestrialColorGenerator* color_Generator)
{
	int triIndex = 0;

	Data.uv.Empty();
	Data.uv.SetNum(Data.Resolution * Data.Resolution);
	for (int y = 0; y < Data.Resolution; y++)
	{
		for (int x = 0; x < Data.Resolution; x++)
		{
			int i = x + y * Data.Resolution;
			FVector2D percent = FVector2D(x, y) / (Data.Resolution - 1);
			FVector pointOnUnitCube = -Data.LocalUp + (percent.X - .5f) * 2 * Data.axisA + (percent.Y - .5f) * 2 * Data.axisB;
			FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
			float unscaledElevation = shapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
			Data.verticies.EmplaceAt(i, pointOnUnitSphere * shapeGenerator->GetScaledElevation(unscaledElevation));
			Data.uv[i].X = color_Generator->BiomePercentFromPoint(pointOnUnitSphere);
			Data.uv[i].Y = unscaledElevation;

			if (x != Data.Resolution - 1 && y != Data.Resolution - 1)
			{
				Data.triangles.Insert(i, triIndex);
				Data.triangles.Insert(i + Data.Resolution + 1, triIndex + 1);
				Data.triangles.Insert(i + Data.Resolution, triIndex + 2);

				Data.triangles.Insert(i, triIndex + 3);
				Data.triangles.Insert(i + 1, triIndex + 4);
				Data.triangles.Insert(i + Data.Resolution + 1, triIndex + 5);

				triIndex += 6;
			}
		}
	}
	ProcMesh->CreateMeshSection(MeshSection, Data.verticies, Data.triangles, Data.normals, Data.uv, Data.VertexColors, Data.tangents, false);
}

void TerrainFace::CalculateMesh()
{
	int triIndex = 0;

	PointsOnUnitSphere.Empty();
	PointsOnUnitSphere.SetNum(Data.Resolution * Data.Resolution);

	Data.uv.Empty();
	Data.uv.SetNum(Data.Resolution * Data.Resolution);
	for (int y = 0; y < Data.Resolution; y++)
	{
		for (int x = 0; x < Data.Resolution; x++)
		{
			int i = x + y * Data.Resolution;
			FVector2D percent = FVector2D(x, y) / (Data.Resolution - 1);
			FVector pointOnUnitCube = -Data.LocalUp + (percent.X - .5f) * 2 * Data.axisA + (percent.Y - .5f) * 2 * Data.axisB;
			FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
			PointsOnUnitSphere[i] = pointOnUnitSphere;
			float unscaledElevation = shapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
			float scaledElevation = shapeGenerator->GetScaledElevation(unscaledElevation);
			Data.verticies.EmplaceAt(i, pointOnUnitSphere * scaledElevation);
			Data.uv[i].X = colorGenerator->BiomePercentFromPoint(pointOnUnitSphere);
			Data.uv[i].Y = unscaledElevation;

			if (x != Data.Resolution - 1 && y != Data.Resolution - 1)
			{
				Data.triangles.Insert(i, triIndex);
				Data.triangles.Insert(i + Data.Resolution + 1, triIndex + 1);
				Data.triangles.Insert(i + Data.Resolution, triIndex + 2);

				Data.triangles.Insert(i, triIndex + 3);
				Data.triangles.Insert(i + 1, triIndex + 4);
				Data.triangles.Insert(i + Data.Resolution + 1, triIndex + 5);

				triIndex += 6;
			}
		}
	}
	UpdateTangentsNormals();
}

void TerrainFace::UpdateBiomePercents()
{
	for (int y = 0; y < Data.Resolution; y++)
	{
		for (int x = 0; x < Data.Resolution; x++)
		{
			int i = x + y * Data.Resolution;

			Data.uv[i].X = colorGenerator->BiomePercentFromPoint(PointsOnUnitSphere[i]);
		}
	}
	ProcMesh->CreateMeshSection(MeshSection, Data.verticies, Data.triangles, Data.normals, Data.uv, Data.VertexColors, Data.tangents, false);
}

void TerrainFace::ConstructMeshAsync(TerrestrialColorGenerator* color_Generator)
{
	//if (Worker)
		//Worker->Stop();
	Worker = new FTerrainFaceWorker(this, Data, colorGenerator, shapeGenerator);
}

void TerrainFace::UpdateTangentsNormals()
{
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Data.verticies, Data.triangles, Data.uv, Data.normals, Data.tangents);
	AsyncTask(ENamedThreads::GameThread, [this]() { CreateMesh(); });
}

void TerrainFace::UpdateTangentsNormalsAsync()
{
	AsyncTask(ENamedThreads::AnyThread, [this]() { UpdateTangentsNormals(); });
}

void TerrainFace::CreateMesh()
{
	ProcMesh->CreateMeshSection(MeshSection, Data.verticies, Data.triangles, Data.normals, Data.uv, Data.VertexColors, Data.tangents, false);
	Cast<APlanet>(Owner)->ReconveneTerrainFaceThreads(MeshSection);
}


// =============== Terrain Face Worker ===============

FTerrainFaceWorker::FTerrainFaceWorker(TerrainFace* IN_Parent, FTerrainFaceData& IN_Data, TerrestrialColorGenerator* IN_ColorGenerator, ShapeGenerator* IN_ShapeGenerator)
	:  Data(IN_Data), ColorGenerator(IN_ColorGenerator), shapeGenerator(IN_ShapeGenerator), Parent(IN_Parent) 
{
    Thread = FRunnableThread::Create(this, TEXT("FTerrainFaceWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

FTerrainFaceWorker::~FTerrainFaceWorker()
{
    delete Thread;
    Thread = NULL;
}

bool FTerrainFaceWorker::Init()
{
    //Init the Data 
	Data.verticies.Empty();
	Data.uv.Empty();
	Data.uv.SetNum(Data.Resolution * Data.Resolution);

    return true;
}

uint32 FTerrainFaceWorker::Run()
{
	int triIndex = 0;

	for (int y = 0; y < Data.Resolution; y++)
	{
		for (int x = 0; x < Data.Resolution; x++)
		{
			if (StopTaskCounter.GetValue() == 0)
			{
				int i = x + y * Data.Resolution;
				FVector2D percent = FVector2D(x, y) / (Data.Resolution - 1);
				FVector pointOnUnitCube = -Data.LocalUp + (percent.X - .5f) * 2 * Data.axisA + (percent.Y - .5f) * 2 * Data.axisB;
				FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
				float unscaledElevation = shapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
				Data.verticies.EmplaceAt(i, pointOnUnitSphere * shapeGenerator->GetScaledElevation(unscaledElevation));
				Data.uv[i].X = ColorGenerator->BiomePercentFromPoint(pointOnUnitSphere);
				Data.uv[i].Y = unscaledElevation;

				if (x != Data.Resolution - 1 && y != Data.Resolution - 1)
				{
					Data.triangles.Insert(i, triIndex);
					Data.triangles.Insert(i + Data.Resolution + 1, triIndex + 1);
					Data.triangles.Insert(i + Data.Resolution, triIndex + 2);

					Data.triangles.Insert(i, triIndex + 3);
					Data.triangles.Insert(i + 1, triIndex + 4);
					Data.triangles.Insert(i + Data.Resolution + 1, triIndex + 5);

					triIndex += 6;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Thread stopped early"));
				return 0;
			}
		}
	}
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Data.verticies, Data.triangles, Data.uv, Data.normals, Data.tangents);
	AsyncTask(ENamedThreads::GameThread, [this]() { Parent->CreateMesh(); });
    return 0;
}

void FTerrainFaceWorker::Stop()
{
    StopTaskCounter.Increment();
}

void FTerrainFaceWorker::EnsureCompletion()
{
    Stop();
    Thread->WaitForCompletion();
}
