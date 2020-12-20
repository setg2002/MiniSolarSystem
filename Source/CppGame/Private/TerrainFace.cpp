// This is a copyright notice


#include "TerrainFace.h"
#include "Planet.h"
#include "AssetRegistryModule.h"
#include "G:\UESource\Engine\Source\Runtime\RawMesh\Public\RawMesh.h"
#include "G:\UESource\Engine\Plugins\Runtime\ProceduralMeshComponent\Source\ProceduralMeshComponentEditor\Private\ProceduralMeshComponentDetails.h"
#include "G:\UESource\Engine\Source\Runtime\Core\Public\HAL\IConsoleManager.h"

TerrainFace::TerrainFace(ShapeGenerator* shape_Generator, ColorGenerator* color_Generator, int resolution, FVector localUp, UProceduralMeshComponent* procMesh, UStaticMeshComponent* staticMesh, int32 meshNum)
{
	colorGenerator = color_Generator;
	shapeGenerator = shape_Generator;
	Resolution = resolution;
	MeshNum = meshNum;
	LocalUp = localUp;
	ProcMesh = procMesh;
	StaticMesh = staticMesh;

	axisA = FVector(this->LocalUp.Y, this->LocalUp.Z, this->LocalUp.X);
	axisB = FVector().CrossProduct(this->LocalUp, axisA);
}

TerrainFace::~TerrainFace()
{
}

void TerrainFace::ConstructMesh(ColorGenerator* color_Generator)
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
	ProcMesh->CreateMeshSection(0, verticies, triangles, normals, uv, VertexColors, tangents, false);
}

void TerrainFace::CalculateMesh()
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

void TerrainFace::ConstructMeshAsync(ColorGenerator* color_Generator)
{
	TArray<FVector>& vertsRef = verticies;
	TArray<int>& trisRef = triangles;
	TArray<FVector2D>& uvRef = uv;
	
	(new FAutoDeleteAsyncTask<CalculateMeshAsyncTask>(*this, Resolution, LocalUp, axisA, axisB, vertsRef, trisRef, uvRef, shapeGenerator, colorGenerator))->StartBackgroundTask();
}

void TerrainFace::CreateMesh(/*TArray<FVector> Verticies, TArray<int> Triangles, TArray<FVector2D> Uv, TArray<FVector> Normals, TArray<FProcMeshTangent> Tangents*/)
{
	ProcMesh->CreateMeshSection(0, verticies, triangles, normals, uv, VertexColors, TArray<FProcMeshTangent>(), false);
	ConvertToStaticMesh("abcdefg");
	//Cast<APlanet>(Mesh->GetOwner())->ConvertAndSetStaticMeshes(MeshNum);
}

void TerrainFace::UpdateTangentsNormals()
{
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(verticies, triangles, uv, normals, tangents);
	CreateMesh(/*verticies, triangles, uv, normals, tangents*/);
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

UStaticMesh* TerrainFace::ConvertToStaticMesh(FString ActorName)
{
	UProceduralMeshComponent* ProcMeshComp = ProcMesh;
	if (ProcMeshComp != nullptr)
	{
		FString AssetName = FString(TEXT("SM_")) + ActorName + FString(TEXT("_")) + FString::FromInt(MeshNum);
		FString PathName = FString(TEXT("/Game/TEST_Meshes/"));
		FString PackageName = PathName + AssetName;

		// Raw mesh data we are filling in
		FRawMesh RawMesh;
		// Materials to apply to new mesh
		TArray<UMaterialInterface*> MeshMaterials;

		int32 VertexBase = 0;

		FProcMeshSection* ProcSection = ProcMeshComp->GetProcMeshSection(0);

		// Copy verts
		for (FProcMeshVertex& Vert : ProcSection->ProcVertexBuffer)
		{
			RawMesh.VertexPositions.Add(Vert.Position);
		}

		// Copy 'wedge' info
		int32 NumIndices = ProcSection->ProcIndexBuffer.Num();
		for (int32 IndexIdx = 0; IndexIdx < NumIndices; IndexIdx++)
		{
			int32 Index = ProcSection->ProcIndexBuffer[IndexIdx];

			RawMesh.WedgeIndices.Add(Index + VertexBase);

			FProcMeshVertex& ProcVertex = ProcSection->ProcVertexBuffer[Index];

			FVector TangentX = ProcVertex.Tangent.TangentX;
			FVector TangentZ = ProcVertex.Normal;
			FVector TangentY = (TangentX ^ TangentZ).GetSafeNormal() * (ProcVertex.Tangent.bFlipTangentY ? -1.f : 1.f);

			RawMesh.WedgeTangentX.Add(TangentX);
			RawMesh.WedgeTangentY.Add(TangentY);
			RawMesh.WedgeTangentZ.Add(TangentZ);

			RawMesh.WedgeTexCoords[0].Add(ProcVertex.UV0);
			RawMesh.WedgeColors.Add(ProcVertex.Color);
		}

		// copy face info
		int32 NumTris = NumIndices / 3;
		for (int32 TriIdx = 0; TriIdx < NumTris; TriIdx++)
		{
			RawMesh.FaceMaterialIndices.Add(0);
			RawMesh.FaceSmoothingMasks.Add(0); // Assume this is ignored as bRecomputeNormals is false
		}

		// Remember material
		MeshMaterials.Add(ProcMeshComp->GetMaterial(0));

		// Update offset for creating one big index/vertex buffer
		VertexBase += ProcSection->ProcVertexBuffer.Num();

		// If we got some valid data.
		if (RawMesh.VertexPositions.Num() > 3 && RawMesh.WedgeIndices.Num() > 3)
		{
			// Then find/create it.
			UPackage* Package = CreatePackage(NULL, *PackageName);
			check(Package);

			// Create StaticMesh object
			UStaticMesh* NewMesh = NewObject<UStaticMesh>(Package, FName(*AssetName), RF_Public | RF_Standalone);
			NewMesh->InitResources();

			NewMesh->LightingGuid = FGuid::NewGuid();

			// Add source to new StaticMesh
			FStaticMeshSourceModel& SrcModel = NewMesh->AddSourceModel();
			SrcModel.BuildSettings.bRecomputeNormals = false;
			SrcModel.BuildSettings.bRecomputeTangents = false;
			SrcModel.BuildSettings.bRemoveDegenerates = false;
			SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
			SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
			SrcModel.BuildSettings.bGenerateLightmapUVs = true;
			SrcModel.BuildSettings.SrcLightmapIndex = 0;
			SrcModel.BuildSettings.DstLightmapIndex = 1;
			SrcModel.RawMeshBulkData->SaveRawMesh(RawMesh);

			// Copy materials to new mesh
			for (UMaterialInterface* Material : MeshMaterials)
			{
				NewMesh->StaticMaterials.Add(FStaticMaterial(Material));
			}

			//Set the Imported version before calling the build
			NewMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;

			// Build mesh from source
			NewMesh->Build(false);
			NewMesh->PostEditChange();

			// Notify asset registry of new asset
			FAssetRegistryModule::AssetCreated(NewMesh);
			
			ProcMesh->ClearMeshSection(0);
			StaticMesh->SetStaticMesh(NewMesh);
			return NewMesh;
		}
	}
	return nullptr;
}
