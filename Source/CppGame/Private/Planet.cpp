// This is a copyright notice


#include "Planet.h"
#include "RawMesh.h"
#include "EngineUtils.h"
#include "Engine/DataAsset.h"
#include "OrbitDebugActor.h"
#include "TerrainFace.h"
#include "ColorSettings.h"
#include "ColorGenerator.h"
#include "ShapeSettings.h"
#include "ShapeGenerator.h"
#include "AssetRegistryModule.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


APlanet::APlanet()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	ProcMeshes = { CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh"),
			   CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh1"),
			   CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh2"),
			   CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh3"),
			   CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh4"),
			   CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh5")
	};

	shapeGenerator = new ShapeGenerator();
	colorGenerator = new TerrestrialColorGenerator(this);
}

void APlanet::OnConstruction(const FTransform & Transform)
{
	// Get orbit debug actor from world
	for (TActorIterator<AOrbitDebugActor> It(GetWorld()); It; ++It)
	{
		OrbitDebugActor = *It;
	}

	Super::OnConstruction(Transform);
}

UDataAsset* APlanet::CreateSettingsAsset(TSubclassOf<UDataAsset> DataAssetClass)
{
	FString AssetPath = FString("/Game/DataAssets/" + this->GetName() + "/");
	FString AssetName = FString(TEXT("DA_")) + this->GetName() + FString(TEXT("_")) + DataAssetClass.Get()->GetName();
	FString PackagePath = AssetPath + AssetName;
	

	UPackage *Package = CreatePackage(*PackagePath);
	UDataAsset* NewDataAsset = NewObject<UDataAsset>(Package, DataAssetClass.Get(), *AssetName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);

	FAssetRegistryModule::AssetCreated(NewDataAsset);
	NewDataAsset->MarkPackageDirty();

	FString FilePath = FString::Printf(TEXT("%s%s%s"), *AssetPath, *AssetName, *FPackageName::GetAssetPackageExtension());
	bool bSuccess = UPackage::SavePackage(Package, NewDataAsset, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *FilePath);
	UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), bSuccess ? TEXT("True") : TEXT("False"));

	return NewDataAsset;
}

void APlanet::CreatePackageName(FString& OutAssetName, FString& OutPackagePath, UObject& OutOuter, TSubclassOf<UDataAsset> DataAssetClass)
{
	FString AssetPath = FString("/Game/DataAssets/" + this->GetName() + "/");
	OutAssetName = FString(TEXT("DA_")) + this->GetName() + FString(TEXT("_")) + DataAssetClass.Get()->GetName();
	OutPackagePath = AssetPath + OutAssetName;
	UObject* OuterPtr = &OutOuter;
	OuterPtr = FindPackage(this, *OutPackagePath);
	return;
}

void APlanet::CreateSettingsAssets()
{
	FString AssetName;
	FString PackagePath;
	UObject* Outer = nullptr;

	if (ColorSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->GetName() + "/" + "DA_" + this->GetName() + "_" + UColorSettings::StaticClass()->GetName())))
	{
		CreatePackageName(AssetName, PackagePath, *Outer, UColorSettings::StaticClass());
		ColorSettings = LoadObject<UColorSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ColorSettings == nullptr)
	{
		ColorSettings = Cast<UColorSettings>(CreateSettingsAsset(UColorSettings::StaticClass()));
	}

	if (ColorSettings->BiomeColorSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->GetName() + "/" + "DA_" + this->GetName() + "_" + UBiomeColorSettings::StaticClass()->GetName())))
	{	
		CreatePackageName(AssetName, PackagePath, *Outer, UBiomeColorSettings::StaticClass());
		ColorSettings->BiomeColorSettings = LoadObject<UBiomeColorSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ColorSettings->BiomeColorSettings == nullptr)
	{
		ColorSettings->BiomeColorSettings = Cast<UBiomeColorSettings>(CreateSettingsAsset(UBiomeColorSettings::StaticClass()));
	}

	if ((ColorSettings->BiomeColorSettings->Biomes == TArray<UBiome*>() || ColorSettings->BiomeColorSettings->Biomes[0] == nullptr) && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->GetName() + "/" + "DA_" + this->GetName() + "_" + UBiome::StaticClass()->GetName())))
	{
		ColorSettings->BiomeColorSettings->Biomes.Empty();
		CreatePackageName(AssetName, PackagePath, *Outer, UBiome::StaticClass());
		ColorSettings->BiomeColorSettings->Biomes.Add(LoadObject<UBiome>(Outer, *AssetName, *PackagePath));
	}
	else if (ColorSettings->BiomeColorSettings->Biomes == TArray<UBiome*>() || ColorSettings->BiomeColorSettings->Biomes[0] == nullptr)
	{
		ColorSettings->BiomeColorSettings->Biomes.Empty();
		ColorSettings->BiomeColorSettings->Biomes.Add(Cast<UBiome>(CreateSettingsAsset(UBiome::StaticClass())));
	}


	if (ShapeSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->GetName() + "/" + "DA_" + this->GetName() + "_" + UShapeSettings::StaticClass()->GetName())))
	{
		CreatePackageName(AssetName, PackagePath, *Outer, UShapeSettings::StaticClass());
		ShapeSettings = LoadObject<UShapeSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ShapeSettings == nullptr)
	{
		ShapeSettings = Cast<UShapeSettings>(CreateSettingsAsset(UShapeSettings::StaticClass()));
	}

	if ((ShapeSettings->NoiseLayers == TArray<UNoiseLayer*>() || ShapeSettings->NoiseLayers[0] == nullptr) && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->GetName() + "/" + "DA_" + this->GetName() + "_" + UNoiseLayer::StaticClass()->GetName())))
	{
		ShapeSettings->NoiseLayers.Empty();
		CreatePackageName(AssetName, PackagePath, *Outer, UNoiseLayer::StaticClass());
		ShapeSettings->NoiseLayers.Add(LoadObject<UNoiseLayer>(Outer, *AssetName, *PackagePath));
	}
	else if (ShapeSettings->NoiseLayers == TArray<UNoiseLayer*>() || ShapeSettings->NoiseLayers[0] == nullptr)
	{
		ShapeSettings->NoiseLayers.Empty();
		ShapeSettings->NoiseLayers.Add(Cast<UNoiseLayer>(CreateSettingsAsset(UNoiseLayer::StaticClass())));
	}

	if (ShapeSettings->NoiseLayers[0]->NoiseSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->GetName() + "/" + "DA_" + this->GetName() + "_" + UNoiseSettings::StaticClass()->GetName())))
	{
		CreatePackageName(AssetName, PackagePath, *Outer, UNoiseSettings::StaticClass());
		ShapeSettings->NoiseLayers[0]->NoiseSettings = LoadObject<UNoiseSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ShapeSettings->NoiseLayers[0]->NoiseSettings == nullptr)
	{
		ShapeSettings->NoiseLayers[0]->NoiseSettings = Cast<UNoiseSettings>(CreateSettingsAsset(UNoiseSettings::StaticClass()));
	}
}

void APlanet::GeneratePlanet()
{
	std::fill(std::begin(ThreadsFinished), std::end(ThreadsFinished), 0);
	if (ColorSettings)
	{
		for (auto& Mesh : ProcMeshes)
		{
			Mesh->SetRelativeLocation(FVector().ZeroVector);
		}

		StaticMesh->SetRelativeLocation(FVector().ZeroVector);

		if (ShapeSettings != nullptr && ShapeSettings->GetNoiseLayers())
		{
			Initialize();
			GenerateMesh();
			GenerateColors();
		}
	}
}

void APlanet::ReGenerate()
{
	GeneratePlanet();
}

void APlanet::ReGenerateColors()
{
	OnColorSettingsUpdated();
}

void APlanet::ReGenerateTangents()
{
	if (bMultithreadGeneration)
	{
		for (int i = 0; i < 6; i++)
		{
			TerrainFaces[i]->UpdateTangentsNormalsAsync();
		}
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			TerrainFaces[i]->UpdateTangentsNormals();
		}
	}
}

void APlanet::Initialize()
{
	shapeGenerator->UpdateSettings(ShapeSettings);
	colorGenerator->UpdateSettings(ColorSettings);
		
	for (int32 i = 0; i < 6; i++)
	{
		TerrainFaces[i] = new TerrainFace(shapeGenerator, colorGenerator, resolution, directions[i], ProcMeshes[i], i, this);

		if (ColorSettings->DynamicMaterials[i] == nullptr)
		{
			ColorSettings->DynamicMaterials[i] = ProcMeshes[i]->CreateAndSetMaterialInstanceDynamicFromMaterial(0, ColorSettings->PlanetMat);
		}
	}
}

void APlanet::GenerateMesh()
{
	if (bMultithreadGeneration)
	{
		for (int i = 0; i < 6; i++)
		{
			TerrainFaces[i]->ConstructMeshAsync(colorGenerator);
		}
	}
	else 
	{
		for (int i = 0; i < 6; i++)
		{
			if (ProcMeshes[i]->IsVisibleInEditor())
			{
				TerrainFaces[i]->CalculateMesh();
			}
		}
		colorGenerator->UpdateElevation(shapeGenerator->ElevationMinMax);
	}
}

void APlanet::OnShapeSettingsUpdated()
{
	Initialize();
	GenerateMesh();
}

void APlanet::OnColorSettingsUpdated()
{
	GenerateColors();
}

void APlanet::GenerateColors()
{
	colorGenerator->UpdateColors();

	// Reload the texture
	StaticMesh->GetMaterial(0)->ReloadConfig();

	if (bMultithreadGeneration)
	{
		colorGenerator->UpdateElevation(shapeGenerator->ElevationMinMax);
	}
}

void APlanet::CalculateOrbitVelocity()
{
	if (OrbitingBody == this || OrbitingBody == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OrbitingBody cannot be null or itself"));
		return;
	}
	else
	{
		//TODO Fix orbit velocity calculation
		//UE_LOG(LogTemp, Warning, TEXT("Distance: %d"), (OrbitingBody->GetActorLocation() - this->GetActorLocation()).Size());
		//orbitVelocity = FMath::Sqrt(((/*gameMode->gravitationalConstant*/100 * 100000) * OrbitingBody->mass) / 11110);
		return;
	}
}

void APlanet::SetToOrbit()
{
	//TODO Calculate what portion of the necessary orbit velocity should be applied to each sectopn of the initialVelocity vector
	initialVelocity.Y = orbitVelocity;
}

void APlanet::ConvertAndSetStaticMesh(int32 i)
{
	ThreadsFinished[i] = true;

	for (int32 k = 0; k < 6; k++)
	{
		if (ThreadsFinished[k] == false)
		{
			return;
		}
	}

	StaticMesh->SetStaticMesh(ConvertToStaticMesh());
}

UStaticMesh* APlanet::ConvertToStaticMesh()
{
	FString AssetName = FString(TEXT("SM_")) + this->GetName();
	FString PathName = FString(TEXT("/Game/PlanetMeshes/"));
	FString PackageName = PathName + AssetName;

	// Raw mesh data we are filling in
	FRawMesh RawMesh;

	int32 VertexBase = 0;

	for (auto& ProcMeshComp : ProcMeshes)
	{
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

		// Update offset for creating one big index/vertex buffer
		VertexBase += ProcSection->ProcVertexBuffer.Num();
	}

	// If we got some valid data.
	if (RawMesh.VertexPositions.Num() > 3 && RawMesh.WedgeIndices.Num() > 3)
	{
		// Then find/create it.
		UPackage* MeshPackage = CreatePackage(*PackageName);
		check(MeshPackage);

		// Create StaticMesh object
		UStaticMesh* NewMesh = NewObject<UStaticMesh>(MeshPackage, FName(*AssetName), RF_Public | RF_Standalone);
		NewMesh->InitResources();

		NewMesh->SetLightingGuid(FGuid::NewGuid());

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
		
		FString NewMaterialName = FString(TEXT("M_")) + this->GetName();
		FString MatPackageName = PathName + NewMaterialName;
		UPackage* MaterialPackage = CreatePackage(*MatPackageName);
		check(MaterialPackage);
		
		UMaterialInterface* MeshMaterial = ProcMeshes[0]->GetMaterial(0); // Old material to copy data from
		UMaterialInstanceDynamic* NewMaterial = NewObject<UMaterialInstanceDynamic>(MaterialPackage, FName(*NewMaterialName), RF_Public | RF_Standalone); // New material to fill in

		NewMaterial->Parent = ColorSettings->PlanetMat;

		TArray<FGuid> Guids;

		// Copy all texture parameters
		TArray<FMaterialParameterInfo> TextureParameters;
		MeshMaterial->GetAllTextureParameterInfo(TextureParameters, Guids);
		for (auto& Parameter : TextureParameters)
		{
			UTexture* Texture;
			MeshMaterial->GetTextureParameterValue(FHashedMaterialParameterInfo(Parameter), Texture);
			NewMaterial->SetTextureParameterValueByInfo(Parameter, Texture);
		}

		// Copy all scalar parameters
		TArray<FMaterialParameterInfo> ScalarParameters;
		MeshMaterial->GetAllScalarParameterInfo(ScalarParameters, Guids);
		for (auto& Parameter : ScalarParameters)
		{
			float ScalarValue;
			MeshMaterial->GetScalarParameterValue(FHashedMaterialParameterInfo(Parameter), ScalarValue);
			NewMaterial->SetScalarParameterValueByInfo(Parameter, ScalarValue);
		}

		// Copy all vector parameters
		TArray<FMaterialParameterInfo> VectorParameters;
		MeshMaterial->GetAllVectorParameterInfo(VectorParameters, Guids);
		for (auto& Parameter : VectorParameters)
		{
			FLinearColor VectorValue;
			MeshMaterial->GetVectorParameterValue(FHashedMaterialParameterInfo(Parameter), VectorValue);
			NewMaterial->SetVectorParameterValueByInfo(Parameter, VectorValue);
		}

		NewMaterial->ReloadConfig(); // Reloads the material to apply changes

		FAssetRegistryModule::AssetCreated(NewMaterial);
		NewMaterial->MarkPackageDirty();

		FString MatPackageFileName = FPackageName::LongPackageNameToFilename(MatPackageName, FPackageName::GetAssetPackageExtension());
		bool bSavedMaterial = UPackage::SavePackage(MaterialPackage, NewMaterial, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *MatPackageFileName, GError, nullptr, true, true, SAVE_NoError);

		// Copy material to new mesh
		NewMesh->SetStaticMaterials(TArray<FStaticMaterial>{ NewMaterial });

		// Set the Imported version before calling the build
		NewMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;

		// Build mesh from source
		NewMesh->Build(false);
		NewMesh->PostEditChange();

		// Notify asset registry of new asset
		FAssetRegistryModule::AssetCreated(NewMesh);
		NewMesh->MarkPackageDirty();

		FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
		bool bSavedMesh = UPackage::SavePackage(MeshPackage, NewMesh, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

		for (auto& ProcMesh : ProcMeshes)
		{
			ProcMesh->ClearMeshSection(0);
		}

		return NewMesh;
	}
	return nullptr;
}

void APlanet::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, resolution) && bAutoGenerate)
		{
			GeneratePlanet();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, ShapeSettings) && bAutoGenerate)
		{
			GeneratePlanet();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, ColorSettings) && bAutoGenerate)
		{
			GeneratePlanet();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, FaceRenderMask) && bAutoGenerate)
		{
			GeneratePlanet();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ACelestialBody, mass) || PropertyName == GET_MEMBER_NAME_CHECKED(ACelestialBody, initialVelocity) || PropertyName == GET_MEMBER_NAME_CHECKED(AActor, GetActorLocation()))
		{
			if (OrbitDebugActor && OrbitDebugActor->bAutoDraw)
			{
				OrbitDebugActor->DrawOrbits();
			}
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
