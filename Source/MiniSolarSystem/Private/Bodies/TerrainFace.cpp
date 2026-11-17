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
	CancelGen = MakeShared<UE::Tasks::FCancellationToken, ESPMode::ThreadSafe>();
}

TerrainFace::~TerrainFace()
{
	// Waits for all worker tasks. Pending game-thread tasks see their cancelled token and never touch this face.
	CancelTerrainFaceGeneration();
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
			
			ShapeGenerator->ElevationMinMax.AddValue(unscaledElevation);

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
	
	// Size enough for this section plus one extra row
	int32 ArraySize = ((OutData.Resolution * OutData.Resolution) / TotalThreads) + OutData.Resolution;
	
	OutData.vertices.SetNumUninitialized(ArraySize);
	OutData.uv.SetNumUninitialized(ArraySize);
	OutData.normals.SetNumUninitialized(ArraySize);
	OutData.tangents.SetNumUninitialized(ArraySize);
	int32 YCount = OutData.Resolution / TotalThreads;
	int32 triIndex = 0;
	
	for (int32 y = 0; y < YCount + 1; y++) // Calculate one extra row so that normals and tangents calculation is smooth between sections
	{
		// global y-index
		int32 gy = y + YCount*SectionIdx;
		
		for (int32 x = 0; x < OutData.Resolution; x++)
		{
			int32 i = x + y * OutData.Resolution;
			FVector2D percent = FVector2D(x, gy) / (OutData.Resolution - 1);
			FVector pointOnUnitCube = -OutData.LocalUp + (percent.X - .5f) * 2 * OutData.axisA + (percent.Y - .5f) * 2 * OutData.axisB;
			FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
			
			float unscaledElevation = ShapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
			OutData.vertices[i] = pointOnUnitSphere * ShapeGenerator->GetScaledElevation(unscaledElevation);
			OutData.uv[i].X = ColorGenerator->BiomePercentFromPoint(pointOnUnitSphere);
			OutData.uv[i].Y = unscaledElevation;
			
			if (y < YCount) // Only add points to this parent array that are actually within the current section
			{	
				PointsOnUnitSphere[x + gy * OutData.Resolution] = pointOnUnitSphere;
				OutData.ElevationMinMax.AddValue(unscaledElevation);
			}
			
			if (x != OutData.Resolution - 1 && gy != OutData.Resolution - 1)
			{
				int32 ii = x + gy * OutData.Resolution;
				
				OutData.triangles.Insert(ii, triIndex);
				OutData.triangles.Insert(ii + OutData.Resolution + 1, triIndex + 1);
				OutData.triangles.Insert(ii + OutData.Resolution, triIndex + 2);

				OutData.triangles.Insert(ii, triIndex + 3);
				OutData.triangles.Insert(ii + 1, triIndex + 4);
				OutData.triangles.Insert(ii + OutData.Resolution + 1, triIndex + 5);

				triIndex += 6;
				
				if (y < YCount)
				{
					OutData.NumTris += 2;
				}
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
	if (GenerationTasks.Num() == 0)
	{
		return;
	}

	// Cancel the current generation
	CancelGen->Cancel();

	// Since worker tasks write into this face (PointsOnUnitSphere) right now and read the shape/color generators we need to wait for them.
	// The game-thread tasks are not waited on: they only check their own (now cancelled) token and return, and the
	// section data they hold is freed with the last shared reference.
	for (const UE::Tasks::FTask& Task : GenerationTasks)
	{
		if (Task.IsValid())
		{
			Task.Wait();
		}
	}
	GenerationTasks.Empty();

	// Fresh token for the next generation. Tasks from the cancelled generation keep the old one alive from the shared ptr.
	CancelGen = MakeShared<UE::Tasks::FCancellationToken, ESPMode::ThreadSafe>();
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
	GenerationTasks.Reset(TotalThreads);
	for (int32 i = 0; i < TotalThreads; ++i)
	{
		// Shared by both tasks of this section and freed when the last of them releases it, whether it finished or was cancelled
		TSharedRef<FTerrainFaceData, ESPMode::ThreadSafe> SectionData = MakeShared<FTerrainFaceData, ESPMode::ThreadSafe>();
		SectionData->LocalUp = Data.LocalUp;
		SectionData->axisA = Data.axisA;
		SectionData->axisB = Data.axisB;
		SectionData->Resolution = Data.Resolution;

		// Captured by value so a task always checks the token of the generation it belongs to, even after CancelGen is replaced
		TSharedPtr<UE::Tasks::FCancellationToken, ESPMode::ThreadSafe> Token = CancelGen;

		UE::Tasks::FTask GenerationTask = UE::Tasks::Launch(TEXT("TerrainFaceGeneration"), [this, i, SectionData, Token]()
		{
			if (Token->IsCanceled())
			{
				return;
			}

			CalculateMeshSection(*SectionData, i);

			if (Token->IsCanceled())
			{
				return;
			}

			SCOPE_CYCLE_COUNTER(STAT_ProcMesh_CalcTangents);
			UKismetProceduralMeshLibrary::CalculateTangentsForMesh(SectionData->vertices, SectionData->triangles, SectionData->uv, SectionData->normals, SectionData->tangents, SectionData->triangles[0], i);
		}, LowLevelTasks::ETaskPriority::High );

		// Runs on the game thread. Must not touch `this` before checking the token: the face may have been destroyed
		UE::Tasks::Launch(TEXT("TerrainFaceCreateSection"), [this, i, SectionData, Token]()
		{
			if (Token->IsCanceled())
			{
				return;
			}

			GenerationThreadFinished(*SectionData, i);
		}, UE::Tasks::Prerequisites(GenerationTask), LowLevelTasks::ETaskPriority::Normal, UE::Tasks::EExtendedTaskPriority::GameThreadNormalPri);

		GenerationTasks.Add(GenerationTask);
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

void TerrainFace::GenerationThreadFinished(const FTerrainFaceData& SectionData, int32 ThreadIdx)
{
	// Aggregate terrain face data
	{
		SCOPE_CYCLE_COUNTER(STAT_ProcMesh_CollectMeshData);
		
		// Aggregate elevation min/max from all sections
		ShapeGenerator->ElevationMinMax.Merge(SectionData.ElevationMinMax);

		int32 Count = (SectionData.Resolution * SectionData.Resolution) / TotalThreads;
		int32 StartIndex = ThreadIdx * Count;
		int32 TrisCount = SectionData.NumTris * 3;
		// Every section but the last holds RowsPerSection full rows of triangles, so offset by that rather than this section's own count
		const int32 RowsPerSection = SectionData.Resolution / TotalThreads;
		int32 StartingTriIndex = ThreadIdx * RowsPerSection * (SectionData.Resolution - 1) * 6;

		// Create a view of the specific range in the source arrays
		TConstArrayView<FVector> VertsSourceView(SectionData.vertices.GetData(), Count);
		TConstArrayView<FVector2D> UVSourceView(SectionData.uv.GetData(), Count);
		TConstArrayView<int32> TrianglesSourceView(SectionData.triangles.GetData(), TrisCount);
		TConstArrayView<FVector> NormalsSourceView(SectionData.normals.GetData(), Count);
		TConstArrayView<FProcMeshTangent> TangentsSourceView(SectionData.tangents.GetData(), Count);

		// Copy the data into the target arrays (TargetArray must have enough allocated space)
		FMemory::Memcpy(Data.vertices.GetData() + StartIndex, VertsSourceView.GetData(), Count * sizeof(FVector));
		FMemory::Memcpy(Data.uv.GetData() + StartIndex, UVSourceView.GetData(), Count * sizeof(FVector2D));
		FMemory::Memcpy(Data.triangles.GetData() + StartingTriIndex, TrianglesSourceView.GetData(), TrisCount * sizeof(int32));
		FMemory::Memcpy(Data.normals.GetData() + StartIndex, NormalsSourceView.GetData(), Count * sizeof(FVector));
		FMemory::Memcpy(Data.tangents.GetData() + StartIndex, TangentsSourceView.GetData(), Count * sizeof(FProcMeshTangent));
	}
	
	// Create final mesh when all threads are finished
	FinishedThreads++;
	if (FinishedThreads == TotalThreads)
	{
		TotalThreads = 0;
		GenerationTasks.Empty();
		
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
