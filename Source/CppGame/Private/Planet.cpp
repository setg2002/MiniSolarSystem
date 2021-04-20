// This is a copyright notice


#include "Planet.h"
#include "RawMesh.h"
#include "EngineUtils.h"
#include "TerrainFace.h"
#include "AssetCleaner.h"
#include "ShapeSettings.h"
#include "ColorSettings.h"
#include "ColorGenerator.h"
#include "ShapeGenerator.h"
#include "OrbitDebugActor.h"
#include "Engine/DataAsset.h"
#include "AssetRegistryModule.h"
#include "UObject/PackageReload.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"


APlanet::APlanet()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

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

void APlanet::BeginPlay()
{
	Super::BeginPlay();

}

void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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
		ColorSettings->GetPackage()->MarkPackageDirty();
	}

	if (ColorSettings->BiomeColorSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->GetName() + "/" + "DA_" + this->GetName() + "_" + UBiomeColorSettings::StaticClass()->GetName())))
	{	
		CreatePackageName(AssetName, PackagePath, *Outer, UBiomeColorSettings::StaticClass());
		ColorSettings->BiomeColorSettings = LoadObject<UBiomeColorSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ColorSettings->BiomeColorSettings == nullptr)
	{
		ColorSettings->BiomeColorSettings = Cast<UBiomeColorSettings>(CreateSettingsAsset(UBiomeColorSettings::StaticClass()));
		ColorSettings->BiomeColorSettings->GetPackage()->MarkPackageDirty();
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
		ColorSettings->BiomeColorSettings->Biomes[0]->GetPackage()->MarkPackageDirty();
	}


	if (ShapeSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->GetName() + "/" + "DA_" + this->GetName() + "_" + UShapeSettings::StaticClass()->GetName())))
	{
		CreatePackageName(AssetName, PackagePath, *Outer, UShapeSettings::StaticClass());
		ShapeSettings = LoadObject<UShapeSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ShapeSettings == nullptr)
	{
		ShapeSettings = Cast<UShapeSettings>(CreateSettingsAsset(UShapeSettings::StaticClass()));
		ShapeSettings->GetPackage()->MarkPackageDirty();
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
		ShapeSettings->NoiseLayers[0]->GetPackage()->MarkPackageDirty();
	}

	if (ShapeSettings->NoiseLayers[0]->NoiseSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->GetName() + "/" + "DA_" + this->GetName() + "_" + UNoiseSettings::StaticClass()->GetName())))
	{
		CreatePackageName(AssetName, PackagePath, *Outer, UNoiseSettings::StaticClass());
		ShapeSettings->NoiseLayers[0]->NoiseSettings = LoadObject<UNoiseSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ShapeSettings->NoiseLayers[0]->NoiseSettings == nullptr)
	{
		ShapeSettings->NoiseLayers[0]->NoiseSettings = Cast<UNoiseSettings>(CreateSettingsAsset(UNoiseSettings::StaticClass()));
		ShapeSettings->NoiseLayers[0]->NoiseSettings->GetPackage()->MarkPackageDirty();
	}

	AssetCleaner::CleanDirectory(EDirectoryFilterType::DataAssets);
}

void APlanet::GeneratePlanet()
{
	if (ColorSettings)
	{
		StaticMesh->SetRelativeLocation(FVector().ZeroVector);

		if (ShapeSettings != nullptr && ShapeSettings->GetNoiseLayers())
		{
			if (bMultithreadGeneration)
			{
				Initialize();
				GenerateMesh();
			}
			else
			{
				Initialize();
				GenerateMesh();
				GenerateColors();
			}

		}
	}
	if (!bMultithreadGeneration)
	{
		AssetCleaner::CleanAll();
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

	TArray<UProceduralMeshComponent*> ProcMeshes;
	for (int8 i = 0; i < 6; i++)
	{
		ProcMeshes.Add(NewObject<UProceduralMeshComponent>());
	}

	ColorSettings->DynamicMaterials.Empty();
	ColorSettings->DynamicMaterials.SetNum(6);
	for (int32 i = 0; i < 6; i++)
	{
		TerrainFaces[i] = new TerrainFace(shapeGenerator, colorGenerator, resolution, directions[i], ProcMeshes[i], this);

		ColorSettings->DynamicMaterials[i] = ProcMeshes[i]->CreateAndSetMaterialInstanceDynamicFromMaterial(0, ColorSettings->PlanetMat);
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
			//if (ProcMeshes[i]->IsVisibleInEditor())
			//{
				TerrainFaces[i]->CalculateMesh();
			//}
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
#if WITH_EDITOR
	colorGenerator->UpdateColors();
#endif

	// Reload the texture
	StaticMesh->GetMaterial(0)->LoadConfig(UMaterialInterface::StaticClass());
	//StaticMesh->GetMaterial(0)->ReloadConfig();  // Possible engine crash if material doesn't exist?

	if (bMultithreadGeneration)
	{
		colorGenerator->UpdateElevation(shapeGenerator->ElevationMinMax);
		StaticMesh->GetMaterial(0)->MarkPackageDirty();
		AssetCleaner::CleanAll();
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
		//TODO Replace 100 with gravitational constant from gamemode
		float GM = 100 * (this->mass + OrbitingBody->mass);
		orbitVelocity = FMath::Sqrt(GM);
		return;
	}
}

void APlanet::SetToOrbit()
{
	if (OrbitingBody == this || OrbitingBody == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OrbitingBody cannot be null or itself"));
		return;
	}

	FVector AtPlanet = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), OrbitingBody->GetActorLocation()).Vector();
	FVector Up = StaticMesh->GetUpVector();
	FVector Tangent = FVector().CrossProduct(AtPlanet, Up).GetSafeNormal();

	initialVelocity.X = Tangent.X * -orbitVelocity + OrbitingBody->initialVelocity.X;
	initialVelocity.Y = Tangent.Y * -orbitVelocity + OrbitingBody->initialVelocity.Y;
	initialVelocity.Z = Tangent.Z * -orbitVelocity + OrbitingBody->initialVelocity.Z;

	if (OrbitDebugActor && OrbitDebugActor->bAutoDraw)
	{
		OrbitDebugActor->DrawOrbits();
	}

	// Debugging
	if (bVectorDebug)
	{
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + (AtPlanet * VectorLength), VectorSize, FColor::Red, VectorDuration, VectorThickness);
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + (Up * VectorLength), VectorSize, FColor::Green, VectorDuration, VectorThickness);
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + (Tangent * VectorLength), VectorSize, FColor::Blue, VectorDuration, VectorThickness);
	}
}

void APlanet::ConvertAndSetStaticMesh(UProceduralMeshComponent* NewMesh)
{
	static TArray<UProceduralMeshComponent*> ProcMeshes;
	ProcMeshes.Add(NewMesh);

#if WITH_EDITOR
	if (ProcMeshes.IsValidIndex(5))
	{
		StaticMesh->SetStaticMesh(ConvertToStaticMesh(ProcMeshes));
		StaticMesh->MarkPackageDirty();
		ProcMeshes.Empty();
		if (bMultithreadGeneration)
		{
			GenerateColors();
		}
	}
#endif
}

#if WITH_EDITOR
UStaticMesh* APlanet::ConvertToStaticMesh(TArray<UProceduralMeshComponent*> ProcMeshes)
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
		MaterialPackage->FullyLoad();
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

		ColorSettings->DynamicMaterials = { NewMaterial };

		// Copy material to new mesh
		NewMesh->SetStaticMaterials(TArray<FStaticMaterial>{ NewMaterial });

		FString MatPackageFileName = FPackageName::LongPackageNameToFilename(MatPackageName, FPackageName::GetAssetPackageExtension());
		bool bSavedMaterial = UPackage::SavePackage(MaterialPackage, NewMaterial, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *MatPackageFileName, GError, nullptr, true, true, SAVE_NoError);

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

		return NewMesh;
	}
	return nullptr;
}


void APlanet::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

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
			OnShapeSettingsUpdated();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, ColorSettings) && bAutoGenerate)
		{
			OnColorSettingsUpdated();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}
		/*if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, FaceRenderMask) && bAutoGenerate)
		{
			GeneratePlanet();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}*/
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ACelestialBody, mass))
		{
			if (OrbitDebugActor && OrbitDebugActor->bAutoDraw)
			{
				OrbitDebugActor->DrawOrbits();
			}
		}
	}
}

void APlanet::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (OrbitDebugActor && OrbitDebugActor->bAutoDraw)
	{
		OrbitDebugActor->DrawOrbits();
	}
}
#endif
