// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "ShapeGenerator.h"
#include "ColorGenerator.h"
#include "ColorSettings.h"
#include "Async/Async.h"

/**
 * 
 */


class CPPGAME_API TerrainFace
{
public:
	TerrainFace(ShapeGenerator* shape_Generator, TerrestrialColorGenerator* color_Generator, int resolution, FVector localUp, UProceduralMeshComponent* mesh, AActor* owner);
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

	int Resolution;
	FVector LocalUp;
	FVector axisA;
	FVector axisB;

	void ConstructMesh(TerrestrialColorGenerator* colorGenerator);
	void ConstructMeshAsync(TerrestrialColorGenerator* colorGenerator);

	void CalculateMesh();

	void CreateMesh();
	void UpdateTangentsNormals();
	void UpdateTangentsNormalsAsync();
};

//================================================================================================================

namespace ThreadingCalculations
{
	static void CalculateMesh(int resolution, FVector localUp, FVector axisA, FVector axisB, TArray<FVector>& verticies, TArray<int>& triangles, TArray<FVector2D>& uv, ShapeGenerator* shapeGenerator, TerrestrialColorGenerator* colorGenerator)
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

	static void CalculateTangents(TArray<FVector>& verticies, TArray<int>& triangles, TArray<FVector2D>& uvs, TArray<FVector>& normals, TArray<FProcMeshTangent>& tangents)
	{
		checkf(&verticies, TEXT("Mesh has no verticies"));
		UKismetProceduralMeshLibrary::CalculateTangentsForMesh(verticies, triangles, uvs, normals, tangents);
		return;
	}
}

//================================================================================================================

class CalculateMeshAsyncTask : public FNonAbandonableTask
{
	TerrainFace& TF;

	int resolution;
	FVector localUp;
	FVector axisA;
	FVector axisB;

	TArray<FColor> VertexColors;

	TArray<FVector>& verticies;
	TArray<int>& triangles;
	TArray<FVector2D>& uv;
	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;

	ShapeGenerator* shapeGenerator;
	TerrestrialColorGenerator* colorGenerator;

public:
	CalculateMeshAsyncTask(TerrainFace& terrainFace, int Resolution, FVector LocalUp, FVector AxisA, FVector AxisB, TArray<FVector>& Verticies, TArray<int>& Triangles, TArray<FVector2D>& Uv, ShapeGenerator* shape_Generator, TerrestrialColorGenerator* color_Generator)
		: TF(terrainFace), verticies(Verticies), triangles(Triangles), uv(Uv)
	{
		resolution = Resolution;
		localUp = LocalUp;
		axisA = AxisA;
		axisB = AxisB;
		shapeGenerator = shape_Generator;
		colorGenerator = color_Generator;
	}

	~CalculateMeshAsyncTask()
	{
		AsyncTask(ENamedThreads::GameThread, [this]() { TF.UpdateTangentsNormalsAsync();});

		UE_LOG(LogTemp, Log, TEXT("Terrain face task finished calculating. Destroying task."));
	}

	void DoWork()
	{
		UE_LOG(LogTemp, Log, TEXT("Calculating Mesh..."));
		ThreadingCalculations::CalculateMesh(resolution, localUp, axisA, axisB, verticies, triangles, uv, shapeGenerator, colorGenerator);
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(CalculateMeshAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};

class CalculateTangentsAsyncTask : public FNonAbandonableTask
{
	TerrainFace& TF;

	TArray<FVector>& verticies;
	TArray<int>& triangles;
	TArray<FVector2D>& uvs;
	TArray<FVector>& normals;
	TArray<FProcMeshTangent>& tangents;

public:
	CalculateTangentsAsyncTask(TerrainFace& terrainFace, TArray<FVector>& Verticies, TArray<int>& Triangles, TArray<FVector2D>& Uvs, TArray<FVector>& Normals, TArray<FProcMeshTangent>& Tangents)
		: TF(terrainFace), verticies(Verticies), triangles(Triangles), uvs(Uvs), normals(Normals), tangents(Tangents)
	{

	}

	~CalculateTangentsAsyncTask()
	{
		AsyncTask(ENamedThreads::GameThread, [this]() { TF.CreateMesh(); });
		UE_LOG(LogTemp, Log, TEXT("Tangents finished calculating. Destroying task."));
	}

	void DoWork()
	{
		UE_LOG(LogTemp, Log, TEXT("Calculating Tangents..."));
		ThreadingCalculations::CalculateTangents(verticies, triangles, uvs, normals, tangents);
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(CalculateTangentsMeshAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};
