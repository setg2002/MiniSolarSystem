// Copyright Soren Gilbertson


#include "Planet.h"
#include "RawMesh.h"
#include "EngineUtils.h"
#include "TerrainFace.h"
#include "ShapeSettings.h"
#include "ColorSettings.h"
#include "ColorGenerator.h"
#include "ShapeGenerator.h"
#include "OrbitDebugActor.h"
#include "MeshDescription.h"
#include "Engine/DataAsset.h"
#include "AssetRegistryModule.h"
#include "UObject/PackageReload.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"


APlanet::APlanet()
{
	//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh");

	shapeGenerator = new ShapeGenerator();
	colorGenerator = new TerrestrialColorGenerator(this);
}

void APlanet::OnConstruction(const FTransform & Transform)
{
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

void APlanet::ResetPosition()
{
	ProcMesh->SetRelativeLocation(FVector::ZeroVector);
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

	//AssetCleaner::CleanDirectory(EDirectoryFilterType::DataAssets);
}

void APlanet::GeneratePlanet()
{
	if (ColorSettings)
	{
		ProcMesh->SetRelativeLocation(FVector().ZeroVector);

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
		//AssetCleaner::CleanAll();
	}

}

void APlanet::ClearMeshSections()
{
	for (int i = 0; i < 6; i++)
	{
		ProcMesh->ClearMeshSection(i);
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

	ColorSettings->DynamicMaterial = UMaterialInstanceDynamic::Create(ColorSettings->PlanetMat, this);
	for (int32 i = 0; i < 6; i++)
	{
		TerrainFaces[i] = new TerrainFace(i, shapeGenerator, colorGenerator, resolution, directions[i], ProcMesh, this);
		ProcMesh->SetMaterial(i, ColorSettings->DynamicMaterial);
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
			if ((int)FaceRenderMask - 1 == i || FaceRenderMask == EFaceRenderMask::All)
			{
				TerrainFaces[i]->CalculateMesh();
			}
			else
			{
				ProcMesh->ClearMeshSection(i);
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
	ProcMesh->GetMaterial(0)->LoadConfig();
	//ProcMesh->GetMaterial(0)->ReloadConfig();  // Possible engine crash if material doesn't exist?

	if (bMultithreadGeneration)
	{
		colorGenerator->UpdateElevation(shapeGenerator->ElevationMinMax);
		//AssetCleaner::CleanAll();
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
		float GM = 100 * (this->mass + OrbitingBody->GetMass());
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
	FVector Up = ProcMesh->GetUpVector();
	FVector Tangent = FVector().CrossProduct(AtPlanet, Up).GetSafeNormal();

	initialVelocity.X = Tangent.X * -orbitVelocity + OrbitingBody->initialVelocity.X;
	initialVelocity.Y = Tangent.Y * -orbitVelocity + OrbitingBody->initialVelocity.Y;
	initialVelocity.Z = Tangent.Z * -orbitVelocity + OrbitingBody->initialVelocity.Z;

	if (AOrbitDebugActor::Get()->bAutoDraw)
	{
		AOrbitDebugActor::Get()->DrawOrbits();
	}

	// Debugging
	if (bVectorDebug)
	{
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + (AtPlanet * VectorLength), VectorSize, FColor::Red, VectorDuration, VectorThickness);
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + (Up * VectorLength), VectorSize, FColor::Green, VectorDuration, VectorThickness);
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + (Tangent * VectorLength), VectorSize, FColor::Blue, VectorDuration, VectorThickness);
	}
}

void APlanet::ReconveneTerrainFaceThreads(int FaceNum)
{
	static TArray<int> FinishedFaces;
	FinishedFaces.Add(FaceNum);

	if (FinishedFaces.IsValidIndex(5) && bMultithreadGeneration)
	{
		GenerateColors();
	}
}

#if WITH_EDITOR
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
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ACelestialBody, GetMass()))
		{
			if (AOrbitDebugActor::Get()->bAutoDraw)
			{
				AOrbitDebugActor::Get()->DrawOrbits();
			}
		}
	}
}

void APlanet::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (AOrbitDebugActor::Get()->bAutoDraw)
	{
		AOrbitDebugActor::Get()->DrawOrbits();
	}
}
#endif
