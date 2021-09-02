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
};


class MINISOLARSYSTEM_API TerrainFace
{
public:
	TerrainFace(int32 i, ShapeGenerator* shape_Generator, TerrestrialColorGenerator* color_Generator, int32 resolution, FVector localUp, UProceduralMeshComponent* mesh, AActor* owner);
	~TerrainFace();

	AActor* Owner;

    FTerrainFaceData Data;

	UProceduralMeshComponent* ProcMesh;

	TerrestrialColorGenerator* colorGenerator;
	ShapeGenerator* shapeGenerator;

    class FTerrainFaceWorker* Worker;

	int32 MeshSection;

	void ConstructMesh(TerrestrialColorGenerator* colorGenerator);
	void ConstructMeshAsync(TerrestrialColorGenerator* colorGenerator);

	void CalculateMesh();

	void UpdateBiomePercents();

	void CreateMesh();
	void UpdateTangentsNormals();
	void UpdateTangentsNormalsAsync();

private:
	TArray<FVector> PointsOnUnitSphere;
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

    //Done?
    bool IsFinished() const
    {
        return Data.verticies.Num() == FMath::Square(Data.Resolution);
    }

    //~~~ Thread Core Functions ~~~

public:
    //Constructor / Destructor
    FTerrainFaceWorker(TerrainFace* IN_Parent, FTerrainFaceData& IN_Data, TerrestrialColorGenerator* IN_ColorGenerator, ShapeGenerator* IN_ShapeGenerator);
    ~FTerrainFaceWorker();

    // Begin FRunnable interface.
    virtual bool Init();
    virtual uint32 Run();
    virtual void Stop();
    // End FRunnable interface

    /** Makes sure this thread has stopped properly */
    void EnsureCompletion();
};
