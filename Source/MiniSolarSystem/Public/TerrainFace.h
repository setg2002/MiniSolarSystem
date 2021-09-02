// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

struct FProcMeshTangent;

class TerrestrialColorGenerator;
class UProceduralMeshComponent;
class ShapeGenerator;
class AActor;


struct FTerrainFaceData
{
    TArray<FVector> verticies;
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
        verticies = TArray<FVector>();
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
        verticies = TArray<FVector>();
        triangles = TArray<int32>();
        uv = TArray<FVector2D>();
        normals = TArray<FVector>();
        tangents = TArray<FProcMeshTangent>();
        VertexColors = TArray<FColor>();
    }

    FTerrainFaceData(TArray<FVector> IN_Verticies, TArray<int32> IN_Triangles, TArray<FVector2D> IN_uv, TArray<FVector> IN_Normals, TArray<FProcMeshTangent> IN_Tangents)
        : verticies(IN_Verticies), 
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
	TerrainFace(int8 FaceMeshSection, ShapeGenerator* shape_Generator, TerrestrialColorGenerator* color_Generator, int32 resolution, FVector localUp, UProceduralMeshComponent* mesh);
	~TerrainFace();

    FTerrainFaceData Data;

    void UpdateResolution(int32 NewResolution);

	UProceduralMeshComponent* ProcMesh;

	TerrestrialColorGenerator* colorGenerator;
	ShapeGenerator* shapeGenerator;

    class FTerrainFaceWorker* Worker;

	int8 MeshSection;

	void ConstructMeshAsync(TerrestrialColorGenerator* colorGenerator);

	void CalculateMesh();

	void UpdateBiomePercents();

	void CreateMesh();
	void UpdateTangentsNormals();
	void UpdateTangentsNormalsAsync();

    bool GetIsFinished() const { return bFinished; }

private:
	TArray<FVector> PointsOnUnitSphere;

    bool bFinished;
};


//~~~~~ Multi Threading ~~~~~
class FTerrainFaceWorker : public FRunnable
{
    /** Thread to run the worker FRunnable on */
    FRunnableThread* Thread;

    /** The Terrain Face Data to Fill */
    FTerrainFaceData& Data;

    TerrestrialColorGenerator* ColorGenerator;
    ShapeGenerator* shapeGenerator;
    TerrainFace* Parent;

    /** Stop this thread? Uses Thread Safe Counter */
    FThreadSafeCounter StopTaskCounter;

    bool bGenerateTangentsNormalsOnly;

public:
    //Done?
    bool IsFinished() const { return Parent->GetIsFinished(); }

    //~~~ Thread Core Functions ~~~

    //Constructor / Destructor
    FTerrainFaceWorker(TerrainFace* IN_Parent, FTerrainFaceData& IN_Data, TerrestrialColorGenerator* IN_ColorGenerator, ShapeGenerator* IN_ShapeGenerator, bool GenerateTangentsNormalsOnly);
    ~FTerrainFaceWorker();

    // Begin FRunnable interface.
    virtual bool Init();
    virtual uint32 Run();
    virtual void Stop();
    // End FRunnable interface

    /** Makes sure this thread has stopped properly */
    void EnsureCompletion();
};
