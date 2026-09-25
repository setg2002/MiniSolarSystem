// Copyright Soren Gilbertson


#include "Bodies/TerrainFace.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "Shape/ShapeGenerator.h"
#include "Color/ColorGenerator.h"

DECLARE_STATS_GROUP(TEXT("TerrainFace"), STATGROUP_TerrainFace, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Calc Mesh Section"), STAT_ProcMesh_CalcMeshSection, STATGROUP_TerrainFace);
DECLARE_CYCLE_STAT(TEXT("Calc Tangents"), STAT_ProcMesh_CalcTangents, STATGROUP_TerrainFace);
DECLARE_CYCLE_STAT(TEXT("Collect Mesh Data"), STAT_ProcMesh_CollectMeshData, STATGROUP_TerrainFace);
DECLARE_CYCLE_STAT(TEXT("Create Mesh Section"), STAT_ProcMesh_CreateMeshSection, STATGROUP_TerrainFace);


TerrainFace::TerrainFace(int8 FaceMeshSection, FShapeGenerator* shape_Generator, FTerrestrialColorGenerator* color_Generator, int32 resolution, FVector localUp, UProceduralMeshComponent* procMesh)
	: ProcMesh(procMesh), ColorGenerator(color_Generator), ShapeGenerator(shape_Generator), MeshSection(FaceMeshSection), bFinished(false)
{
	Data = FTerrainFaceData(resolution, localUp);
	CancelGen = new UE::Tasks::FCancellationToken();
}

TerrainFace::~TerrainFace()
{
	CancelTerrainFaceGeneration();
	if (CancelGen)
	{
		delete CancelGen;
	}	
}

void TerrainFace::UpdateResolution(int32 NewResolution)
{
	FVector LocalUp = Data.LocalUp;
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
			float unscaledElevation = ShapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
			float scaledElevation = ShapeGenerator->GetScaledElevation(unscaledElevation);
			Data.vertices.EmplaceAt(i, pointOnUnitSphere * scaledElevation);
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
	}
	UpdateTangentsNormals();
}

void TerrainFace::CalculateMeshSection(FTerrainFaceData& OutData, int32 SectionIdx)
{
	SCOPE_CYCLE_COUNTER(STAT_ProcMesh_CalcMeshSection);
	
	OutData.vertices.SetNum((OutData.Resolution * OutData.Resolution) / TotalThreads);
	OutData.uv.SetNum((OutData.Resolution * OutData.Resolution) / TotalThreads);
	OutData.normals.SetNum((OutData.Resolution * OutData.Resolution) / TotalThreads);
	OutData.tangents.SetNum((OutData.Resolution * OutData.Resolution) / TotalThreads);
	int32 triIndex = 0;
	int32 YCount = OutData.Resolution / TotalThreads;
	
	for (int32 y = 0; y < YCount; y++)
	{
		for (int32 x = 0; x < OutData.Resolution; x++)
		{
			int32 i = x + y * OutData.Resolution;
			FVector2D percent = FVector2D(x, y + (YCount * SectionIdx)) / (OutData.Resolution - 1);
			FVector pointOnUnitCube = -OutData.LocalUp + (percent.X - .5f) * 2 * OutData.axisA + (percent.Y - .5f) * 2 * OutData.axisB;
			FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
			PointsOnUnitSphere[i] = pointOnUnitSphere;
			float unscaledElevation = ShapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
			OutData.vertices[i] = pointOnUnitSphere * ShapeGenerator->GetScaledElevation(unscaledElevation);
			OutData.uv[i].X = ColorGenerator->BiomePercentFromPoint(pointOnUnitSphere);
			OutData.uv[i].Y = unscaledElevation;

			if (x != OutData.Resolution - 1 && y != OutData.Resolution - 1)
			{
				int32 ii = x + (y + (YCount * SectionIdx)) * OutData.Resolution;
				
				OutData.triangles.Insert(ii, triIndex);
				OutData.triangles.Insert(ii + OutData.Resolution + 1, triIndex + 1);
				OutData.triangles.Insert(ii + OutData.Resolution, triIndex + 2);

				OutData.triangles.Insert(ii, triIndex + 3);
				OutData.triangles.Insert(ii + 1, triIndex + 4);
				OutData.triangles.Insert(ii + OutData.Resolution + 1, triIndex + 5);

				triIndex += 6;
			}
		}
	}
}

void TerrainFace::UpdateBiomePercents()
{
	for (int y = 0; y < Data.Resolution; y++)
	{
		for (int x = 0; x < Data.Resolution; x++)
		{
			int i = x + y * Data.Resolution;

			Data.uv[i].X = ColorGenerator->BiomePercentFromPoint(PointsOnUnitSphere[i]);
		}
	}
	ProcMesh->UpdateMeshSection(MeshSection, Data.vertices, Data.normals, Data.uv, Data.VertexColors, Data.tangents);
}

void TerrainFace::CancelTerrainFaceGeneration()
{
	if (Tasks.Num() != 0)
	{
		CancelGen->Cancel();
		for (const UE::Tasks::FTask* Task : Tasks)
		{
			if (!Task->IsCompleted())
			{
				Task->Wait();
			}
		}
		Tasks.Empty();
	}
	
	// Create new cancellation token
	if (CancelGen)
	{
		delete CancelGen;
	}
	CancelGen = new UE::Tasks::FCancellationToken();
}

void TerrainFace::ConstructMeshAsync()
{
	CancelTerrainFaceGeneration();	
	
	bFinished = false;
	
	// Preallocate memory for arrays, necessary for aggregating individual thread data in TerrainFace::GenerationThreadFinished
	PointsOnUnitSphere.Empty();
	PointsOnUnitSphere.SetNum(Data.Resolution * Data.Resolution);
	Data.vertices.Empty();
	Data.vertices.SetNum(Data.Resolution * Data.Resolution);
	Data.uv.Empty();
	Data.uv.SetNum(Data.Resolution * Data.Resolution);
	Data.normals.Empty();
	Data.normals.SetNum(Data.Resolution * Data.Resolution);
	Data.tangents.Empty();
	Data.tangents.SetNum(Data.Resolution * Data.Resolution);
	Data.triangles.Empty();
	Data.triangles.SetNum((Data.Resolution - 1) * (Data.Resolution - 1) * 6);
	
	FinishedThreads = 0;
	switch (Data.Resolution) //TODO SG- Make an algorithm to decide thread count per resolution
	{
	case 16:
	case 32:
	case 64:
		TotalThreads = 4;
		break;
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
	
	// Launch tasks for each section of this face
	Tasks.SetNum(TotalThreads);
	for (int i = 0; i < TotalThreads; ++i)
	{
		FTerrainFaceData& SectionData = *new FTerrainFaceData();
		SectionData.LocalUp = Data.LocalUp;
		SectionData.axisA = Data.axisA;
		SectionData.axisB = Data.axisB;
		SectionData.Resolution = Data.Resolution;
		
		UE::Tasks::FTask GenerationTask = UE::Tasks::Launch(GetThreadName(i, true), [this, i, &SectionData]()
		{
			if (CancelGen->IsCanceled())
			{
				delete &SectionData;
				return;
			}
			
			CalculateMeshSection(SectionData, i);
			
			if (CancelGen->IsCanceled())
			{
				delete &SectionData;
				return;
			}
			
			SCOPE_CYCLE_COUNTER(STAT_ProcMesh_CalcTangents);
			UKismetProceduralMeshLibrary::CalculateTangentsForMesh(SectionData.vertices, SectionData.triangles, SectionData.uv, SectionData.normals, SectionData.tangents, i);
		}, LowLevelTasks::ETaskPriority::High );
		
		UE::Tasks::FTask CreateSectionTask = UE::Tasks::Launch(GetThreadName(i, false), [this, i, &SectionData]()
		{
			if (CancelGen->IsCanceled())
			{
				delete &SectionData;
				return;
			}
				
			GenerationThreadFinished(&SectionData, i);	
		}, UE::Tasks::Prerequisites(GenerationTask), LowLevelTasks::ETaskPriority::Normal, UE::Tasks::EExtendedTaskPriority::GameThreadNormalPri);
	
		Tasks[i] = &CreateSectionTask;
	}
}

void TerrainFace::UpdateTangentsNormals()
{
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Data.vertices, Data.triangles, Data.uv, Data.normals, Data.tangents);
	CreateMesh();
}

void TerrainFace::UpdateTangentsNormalsAsync()
{
	//TODO- Implement for sections
	UpdateTangentsNormals();
}

void TerrainFace::GenerationThreadFinished(FTerrainFaceData* SectionData, int32 ThreadIdx)
{
	// Aggregate terrain face data
	{
		SCOPE_CYCLE_COUNTER(STAT_ProcMesh_CollectMeshData);
		
		int32 Count = SectionData->vertices.Num();
		int32 StartIndex = ThreadIdx * Count;
		int32 TrisCount = SectionData->triangles.Num();
		int32 StartingTriIndex = ThreadIdx * TrisCount;
	
		// Create a view of the specific range in the source arrays
		TArrayView<FVector> VertsSourceView(SectionData->vertices.GetData(), Count);
		TArrayView<FVector2D> UVSourceView(SectionData->uv.GetData(), Count);
		TArrayView<int32> TrianglesSourceView(SectionData->triangles.GetData(), TrisCount);
		TArrayView<FVector> NormalsSourceView(SectionData->normals.GetData(), Count);
		TArrayView<FProcMeshTangent> TangentsSourceView(SectionData->tangents.GetData(), Count);
	
		// Copy the data into the target arrays (TargetArray must have enough allocated space)
		FMemory::Memcpy(Data.vertices.GetData() + StartIndex, VertsSourceView.GetData(), Count * sizeof(FVector));
		FMemory::Memcpy(Data.uv.GetData() + StartIndex, UVSourceView.GetData(), Count * sizeof(FVector2D));
		FMemory::Memcpy(Data.triangles.GetData() + StartingTriIndex, TrianglesSourceView.GetData(), TrisCount * sizeof(int32));
		FMemory::Memcpy(Data.normals.GetData() + StartIndex, NormalsSourceView.GetData(), Count * sizeof(FVector));
		FMemory::Memcpy(Data.tangents.GetData() + StartIndex, TangentsSourceView.GetData(), Count * sizeof(FProcMeshTangent));
	
		delete SectionData;
	}
	
	// Create final mesh when all threads are finished
	FinishedThreads++;
	if (FinishedThreads == TotalThreads)
	{
		TotalThreads = 0;
		Tasks.Empty();
		
		CreateMesh();
	}
}

void TerrainFace::CreateMesh()
{
	SCOPE_CYCLE_COUNTER(STAT_ProcMesh_CreateMeshSection);
	TArray<FVector2D> EmptyArray;
	ProcMesh->CreateMeshSection(MeshSection, Data.vertices, Data.triangles, Data.normals, Data.uv, EmptyArray, EmptyArray, EmptyArray, Data.VertexColors, Data.tangents, false);
	bFinished = true;
}

const TCHAR* TerrainFace::GetThreadName(int32 ThreadIdx, bool bGeneration) const
{
	return *FString::Printf(TEXT("%s%sThread%i"), *LocalUpString(Data.LocalUp), bGeneration ? TEXT("Generation") : TEXT("Create"), ThreadIdx);
}

FString TerrainFace::LocalUpString(FVector LocalUp)
{
	if (LocalUp == FVector::UpVector)
	{
		return FString("Up");
	}
	if (LocalUp == FVector::DownVector)
	{
		return FString("Down");
	}
	if (LocalUp == FVector::LeftVector)
	{
		return FString("Left");
	}
	if (LocalUp == FVector::RightVector)
	{
		return FString("Right");
	}
	if (LocalUp == FVector::ForwardVector)
	{
		return FString("Front");
	}
	if (LocalUp == FVector::BackwardVector)
	{
		return FString("Back");
	}
	
	return FString("???");
}

int32 TerrainFace::GetSectionIndex(int32 ThreadIdx) const
{
	int32 LocalUpMultiplier = -1;
	if (Data.LocalUp == FVector::UpVector)
	{
		LocalUpMultiplier = 0;
	}
	if (Data.LocalUp == FVector::DownVector)
	{
		LocalUpMultiplier = 1;
	}
	if (Data.LocalUp == FVector::LeftVector)
	{
		LocalUpMultiplier = 2;
	}
	if (Data.LocalUp == FVector::RightVector)
	{
		LocalUpMultiplier = 3;
	}
	if (Data.LocalUp == FVector::ForwardVector)
	{
		LocalUpMultiplier = 4;
	}
	if (Data.LocalUp == FVector::BackwardVector)
	{
		LocalUpMultiplier = 5;
	}	
	
	return LocalUpMultiplier * TotalThreads + ThreadIdx;
}
