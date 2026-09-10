// Copyright Soren Gilbertson


#include "Bodies/TerrainFace.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "Shape/ShapeGenerator.h"
#include "Color/ColorGenerator.h"


TerrainFace::TerrainFace(int8 FaceMeshSection, ShapeGenerator* shape_Generator, TerrestrialColorGenerator* color_Generator, int32 resolution, FVector localUp, UProceduralMeshComponent* procMesh)
	: ProcMesh(procMesh), colorGenerator(color_Generator), shapeGenerator(shape_Generator), MeshSection(FaceMeshSection), bFinished(false)
{
	Data = FTerrainFaceData(resolution, localUp);
}

TerrainFace::~TerrainFace()
{
	//delete& Data;
	if (Workers.Num() > 0)
	{
		for (FTerrainFaceWorker* Worker : Workers)
		{
			Worker->EnsureCompletion();
		}
	}
}

void TerrainFace::UpdateResolution(int32 NewResolution)
{
	FVector LocalUp = Data.LocalUp;
	//delete& Data;
	Data = FTerrainFaceData(NewResolution, LocalUp);
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
			Data.vertices.EmplaceAt(i, pointOnUnitSphere * scaledElevation);
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
	ProcMesh->UpdateMeshSection(MeshSection, Data.vertices, Data.normals, Data.uv, Data.VertexColors, Data.tangents);
}

void TerrainFace::ConstructMeshAsync(TerrestrialColorGenerator* color_Generator)
{
	//if (!Worker->IsFinished())
		//Worker->Stop();
	
	bFinished = false;
	
	PointsOnUnitSphere.Empty();
	PointsOnUnitSphere.SetNum(Data.Resolution * Data.Resolution);
	Data.vertices.Empty();
	Data.vertices.SetNum(Data.Resolution * Data.Resolution);
	Data.uv.Empty();
	Data.uv.SetNum(Data.Resolution * Data.Resolution);
	Data.triangles.Empty();
	Data.triangles.SetNum((Data.Resolution - 1) * (Data.Resolution - 1) * 6);
	
	FinishedThreads = 0;
	switch (Data.Resolution) //TODO SG- Make an algorithm to decide thread count per resolution
	{
	case 64:
	case 128:
		TotalThreads = 4;
		break;
	case 256:
		TotalThreads = 8;
		break;
	case 1024:
		TotalThreads = 64;
		break;
	default:
		TotalThreads = 1;
		break;
	}
	
	Workers.Empty();
	Workers.SetNum(TotalThreads);
	for (int i = 0; i < TotalThreads; ++i)
	{
		Workers[i] = new FTerrainFaceWorker(this, Data, false, TotalThreads, i, PointsOnUnitSphere, colorGenerator, shapeGenerator);
	}
}

void TerrainFace::UpdateTangentsNormals()
{
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Data.vertices, Data.triangles, Data.uv, Data.normals, Data.tangents);
	AsyncTask(ENamedThreads::GameThread, [this]() { CreateMesh(); });
}

void TerrainFace::UpdateTangentsNormalsAsync()
{
	//if (!Worker->IsFinished())
		//Worker->Stop();
	bFinished = false;
	
	FinishedThreads = 0;
	TotalThreads = 1;

	//TODO SG- Can this Tangents & Normals calculation be made to use multiple threads?
	Workers.Empty();
	Workers.SetNum(TotalThreads);
	Workers[0] = new FTerrainFaceWorker(this, Data, true, TotalThreads, 0, PointsOnUnitSphere);
}

void TerrainFace::ThreadFinished(bool bNeedGenTangentsNormals)
{
	FinishedThreads++;
	if (FinishedThreads == TotalThreads)
	{
		Workers.Empty();
		TotalThreads = 0;
		
		if (bNeedGenTangentsNormals)
		{
			UpdateTangentsNormalsAsync();
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, [this]() { CreateMesh(); });
		}
	}
}

void TerrainFace::CreateMesh()
{
	ProcMesh->CreateMeshSection(MeshSection, Data.vertices, Data.triangles, Data.normals, Data.uv, Data.VertexColors, Data.tangents, false);
	bFinished = true;
}


// =============== Terrain Face Worker ===============

FTerrainFaceWorker::FTerrainFaceWorker(TerrainFace* IN_Parent, FTerrainFaceData& IN_Data, bool GenerateTangentsNormalsOnly, int32 IN_TotalThreads, int32 IN_ThreadIndex, TArray<FVector>& IN_PointsOnUnitSphere, TerrestrialColorGenerator* IN_ColorGenerator, ShapeGenerator* IN_ShapeGenerator)
	:  Data(IN_Data), PointsOnUnitSphere(IN_PointsOnUnitSphere), ColorGenerator(IN_ColorGenerator), shapeGenerator(IN_ShapeGenerator), Parent(IN_Parent), bGenerateTangentsNormalsOnly(GenerateTangentsNormalsOnly), ThreadIndex(IN_ThreadIndex) , TotalThreads(IN_TotalThreads)
{
    Thread = FRunnableThread::Create(this, *FString::Printf(TEXT("FTerrainFaceWorker%s%i"), *Data.LocalUp.ToString(), ThreadIndex), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

FTerrainFaceWorker::~FTerrainFaceWorker()
{
    delete Thread;
    Thread = NULL;
}

bool FTerrainFaceWorker::Init()
{
	if (TotalThreads < 1)
	{
		return false;
	}
	
    return true;
}

uint32 FTerrainFaceWorker::Run()
{
	if (bGenerateTangentsNormalsOnly)
	{
		//	We need to make a copy of Data to pass to the tangents calculation because the game will crash if data is
		// deleted mid-calculation.
		FTerrainFaceData DataCopy = FTerrainFaceData(Data.vertices, Data.triangles, Data.uv, Data.normals, Data.tangents);
		UKismetProceduralMeshLibrary::CalculateTangentsForMesh(DataCopy.vertices, DataCopy.triangles, DataCopy.uv, DataCopy.normals, DataCopy.tangents);
		
		if (StopTaskCounter.GetValue() == 0)
		{
			Data.normals = DataCopy.normals;
			Data.tangents = DataCopy.tangents;
		}
	}
	else
	{
		int32 StartIndex = (Data.Resolution / TotalThreads) * ThreadIndex;
		int32 EndIndex = (Data.Resolution / TotalThreads) + StartIndex; 
		int triIndex = StartIndex * (Data.Resolution - 1) * 6;
		
		for (int y = StartIndex; y < EndIndex; y++)
		{
			for (int x = 0; x < Data.Resolution; x++)
			{
				if (StopTaskCounter.GetValue() == 0)
				{
					int i = x + y * Data.Resolution;
					FVector2D percent = FVector2D(x, y) / (Data.Resolution - 1);
					FVector pointOnUnitCube = -Data.LocalUp + (percent.X - .5f) * 2 * Data.axisA + (percent.Y - .5f) * 2 * Data.axisB;
					FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
					PointsOnUnitSphere[i] = pointOnUnitSphere;
					float unscaledElevation = shapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
					Data.vertices[i] = pointOnUnitSphere * shapeGenerator->GetScaledElevation(unscaledElevation);
					Data.uv[i].X = ColorGenerator->BiomePercentFromPoint(pointOnUnitSphere);
					Data.uv[i].Y = unscaledElevation;

					if (x != Data.Resolution - 1 && y != Data.Resolution - 1)
					{
						Data.triangles[triIndex    ] = i;
						Data.triangles[triIndex + 1] = i + Data.Resolution + 1;
						Data.triangles[triIndex + 2] = i + Data.Resolution;

						Data.triangles[triIndex + 3] = i;
						Data.triangles[triIndex + 4] = i + 1;
						Data.triangles[triIndex + 5] = i + Data.Resolution + 1;

						triIndex += 6;
					}
				}
				else
					return 1;
			}
		}
	}

	if (StopTaskCounter.GetValue() == 0)
	{
		AsyncTask(ENamedThreads::GameThread, [this]() { Parent->ThreadFinished(!bGenerateTangentsNormalsOnly); });
	}
	else
		return 1;

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
