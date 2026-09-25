// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

struct FProcMeshTangent;

class FTerrestrialColorGenerator;
class UProceduralMeshComponent;
class FShapeGenerator;
class AActor;


struct FTerrainFaceData
{
    TArray<FVector> vertices;
    TArray<int32> triangles;
    TArray<FVector2D> uv;
    TArray<FVector> normals;
    TArray<FProcMeshTangent> tangents;
    TArray<FColor> VertexColors;
    int32 Resolution;
    FVector LocalUp;
    FVector axisA;
    FVector axisB;

    FTerrainFaceData()
    {
        LocalUp = FVector::ZeroVector;
        Resolution = 0;
        axisA = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X);
        axisB = FVector().CrossProduct(LocalUp, axisA);
        vertices = TArray<FVector>();
        triangles = TArray<int32>();
        uv = TArray<FVector2D>();
        normals = TArray<FVector>();
        tangents = TArray<FProcMeshTangent>();
        VertexColors = TArray<FColor>();
    }

    FTerrainFaceData(int32 IN_Resolution, FVector IN_LocalUp)
        : Resolution(IN_Resolution), LocalUp(IN_LocalUp)
    {
        axisA = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X);
        axisB = FVector().CrossProduct(LocalUp, axisA);
        vertices = TArray<FVector>();
        triangles = TArray<int32>();
        uv = TArray<FVector2D>();
        normals = TArray<FVector>();
        tangents = TArray<FProcMeshTangent>();
        VertexColors = TArray<FColor>();
    }

    FTerrainFaceData(TArray<FVector> IN_Verticies, TArray<int32> IN_Triangles, TArray<FVector2D> IN_uv, TArray<FVector> IN_Normals, TArray<FProcMeshTangent> IN_Tangents)
        : vertices(IN_Verticies), 
        triangles(IN_Triangles), 
        uv(IN_uv), 
        normals(IN_Normals), 
        tangents(IN_Tangents),
        VertexColors(TArray<FColor>()),
        Resolution(0),
        LocalUp(FVector::ZeroVector),
        axisA(FVector::ZeroVector),
        axisB(FVector::ZeroVector)
    {
    }
};


class MINISOLARSYSTEM_API TerrainFace
{
public:
	TerrainFace(int8 FaceMeshSection, FShapeGenerator* shape_Generator, FTerrestrialColorGenerator* color_Generator, int32 resolution, FVector localUp, UProceduralMeshComponent* mesh);
	~TerrainFace();

    FTerrainFaceData Data;

    void UpdateResolution(int32 NewResolution);

	UProceduralMeshComponent* ProcMesh;

	FTerrestrialColorGenerator* ColorGenerator;
	FShapeGenerator* ShapeGenerator;

	int8 MeshSection;

	void CancelTerrainFaceGeneration();
	void ConstructMeshAsync();

	void CalculateMesh();
	void CalculateMeshSection(FTerrainFaceData& OutData, int32 SectionIdx);

	void UpdateBiomePercents();

	void CreateMesh();
	void UpdateTangentsNormals();
	void UpdateTangentsNormalsAsync();

    bool GetIsFinished() const { return bFinished; }
	
	void GenerationThreadFinished(FTerrainFaceData* SectionData, int32 ThreadIdx);
	

private:
	TArray<FVector> PointsOnUnitSphere;
	
	TArray<UE::Tasks::FTask*> Tasks;

	// Is this face finished generating. When false this face is actively being generated
    bool bFinished;
	
	int32 FinishedThreads = 0;
	int32 TotalThreads = 0;
	
	const TCHAR* GetThreadName(int32 ThreadIdx, bool bGeneration) const;
	int32 GetSectionIndex(int32 ThreadIdx) const;
	
	static FString LocalUpString(FVector LocalUp);
	
	// 
	UE::Tasks::FCancellationToken* CancelGen;
};
