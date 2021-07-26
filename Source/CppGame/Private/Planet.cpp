// Copyright Soren Gilbertson


#include "Planet.h"
#include "RawMesh.h"
#include "EngineUtils.h"
#include "NoiseLayer.h"
#include "TerrainFace.h"
#include "ShapeSettings.h"
#include "ColorSettings.h"
#include "ColorGenerator.h"
#include "ShapeGenerator.h"
#include "OrbitDebugActor.h"
#include "MeshDescription.h"
#include "Misc/PackageName.h"
#include "Engine/DataAsset.h"
#include "AssetRegistryModule.h"
#include "UObject/PackageReload.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CelestialSaveGameArchive.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"


APlanet::APlanet()
{
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh");
	ProcMesh->SetupAttachment(RootComponent);
	ProcMesh->SetRelativeLocation(FVector::ZeroVector);
	bGenerating = false;
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

	CreateSettingsAssets();

	BindDelegates();
	
	//bGenerating = false;
	//GeneratePlanet();

}

void APlanet::BindDelegates()
{
	ShapeSettings->OnSettingsAssetChanged.AddUFunction(this, "OnShapeSettingsUpdated");
	for (auto& NoiseLayer : ShapeSettings->GetNoiseLayers())
	{
		NoiseLayer->OnSettingsAssetChanged.AddUFunction(this, "OnShapeSettingsUpdated");
		NoiseLayer->NoiseSettings->OnSettingsAssetChanged.AddUFunction(this, "OnShapeSettingsUpdated");
	}

	ColorSettings->OnSettingsAssetChanged.AddUFunction(this, "OnColorSettingsUpdated");
	ColorSettings->GetBiomeColorSettings()->OnSettingsAssetChanged.AddUFunction(this, "OnColorSettingsUpdated");
	if (ColorSettings->GetBiomeColorSettings()->GetUsingNoise())
	{
		ColorSettings->GetBiomeColorSettings()->GetNoise()->OnSettingsAssetChanged.AddUFunction(this, "OnColorSettingsUpdated");
	}
	for (auto& Biome : ColorSettings->GetBiomeColorSettings()->GetBiomes())
	{
		Biome->OnSettingsAssetChanged.AddUFunction(this, "OnColorSettingsUpdated");
	}
}

void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlanet::ResetPosition()
{
	ProcMesh->SetRelativeLocation(FVector::ZeroVector);
}

UObject* APlanet::CreateSettingsAsset(TSubclassOf<UObject> AssetClass)
{
	FString AssetPath = FString("/Game/DataAssets/Runtime/" + this->BodyName.ToString() + "/");
	FString AssetName = FString(TEXT("DA_")) + this->BodyName.ToString() + FString(TEXT("_")) + AssetClass.Get()->GetName() + "_0";
	FString PackagePath = AssetPath + AssetName;
	
	int AssetNum = 0;
	bool PackageExists = FindObject<UPackage>(nullptr, *PackagePath) == NULL ? false : true;
	while (PackageExists)
	{
		AssetNum++;
		AssetName = FString(TEXT("DA_")) + this->BodyName.ToString() + FString(TEXT("_")) + AssetClass.Get()->GetName() + "_" + FString::FromInt(AssetNum);
		PackagePath = AssetPath + AssetName;

		PackageExists = FindObject<UPackage>(nullptr, *PackagePath) == NULL ? false : true;
	}

	UPackage *Package = CreatePackage(*PackagePath);
	UObject* NewAsset = NewObject<UObject>(Package, AssetClass.Get(), *AssetName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);

	FAssetRegistryModule::AssetCreated(NewAsset);
	NewAsset->MarkPackageDirty();

	FString FilePath = FString::Printf(TEXT("%s%s%s"), *AssetPath, *AssetName, *FPackageName::GetAssetPackageExtension());
	bool bSuccess = UPackage::SavePackage(Package, NewAsset, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *FilePath);
	UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), bSuccess ? TEXT("True") : TEXT("False"));

	return NewAsset;
}

UObject* APlanet::CreateSettingsAssetEditor(TSubclassOf<UObject> AssetClass)
{
	FString AssetPath = FString("/Game/DataAssets/" + this->BodyName.ToString() + "/");
	FString AssetName = FString(TEXT("DA_")) + this->BodyName.ToString() + FString(TEXT("_")) + AssetClass.Get()->GetName() + "_0";
	FString PackagePath = AssetPath + AssetName;

	int AssetNum = 0;
	bool PackageExists = FindObject<UPackage>(nullptr, *PackagePath) == NULL ? false : true;
	while (PackageExists)
	{
		AssetNum++;
		AssetName = FString(TEXT("DA_")) + this->BodyName.ToString() + FString(TEXT("_")) + AssetClass.Get()->GetName() + "_" + FString::FromInt(AssetNum);
		PackagePath = AssetPath + AssetName;

		PackageExists = FindObject<UPackage>(nullptr, *PackagePath) == NULL ? false : true;
	}

	UPackage *Package = CreatePackage(*PackagePath);
	UObject* NewAsset = NewObject<UObject>(Package, AssetClass.Get(), *AssetName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);

	FAssetRegistryModule::AssetCreated(NewAsset);
	NewAsset->MarkPackageDirty();

	FString FilePath = FString::Printf(TEXT("%s%s%s"), *AssetPath, *AssetName, *FPackageName::GetAssetPackageExtension());
	bool bSuccess = UPackage::SavePackage(Package, NewAsset, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *FilePath);
	UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), bSuccess ? TEXT("True") : TEXT("False"));

	return NewAsset;
}

template< class T >
T* APlanet::RestoreSettingsAsset(FName Name, TArray<uint8> Data)
{
	FString PackageName = TEXT("/Game/DataAssets/Runtime/");
	UPackage* Package = CreatePackage(*PackageName);

	T* NewAsset = NewObject<T>(Package, Name, EObjectFlags::RF_Public);

	if (NewAsset != NULL && Data.Num() > 1)
	{
		// Fill in the asset's data here
		FMemoryReader MemoryReader(Data);
		FCelestialSaveGameArchive Ar(MemoryReader);
		NewAsset->Serialize(Ar);
	}

	FAssetRegistryModule::AssetCreated(NewAsset);
	NewAsset->MarkPackageDirty();
	FString AssetName = Name.ToString();
	FString FilePath = FString::Printf(TEXT("%s%s%s"), *PackageName, *AssetName, *FPackageName::GetAssetPackageExtension());
	UPackage::SavePackage(Package, NewAsset, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *FilePath);
	return NewAsset;
}

void APlanet::CreatePackageName(FString& OutAssetName, FString& OutPackagePath, UObject& OutOuter, TSubclassOf<UDataAsset> DataAssetClass)
{
	FString AssetPath = FString("/Game/DataAssets/" + this->GetBodyName().ToString() + "/");
	OutAssetName = FString(TEXT("DA_")) + this->GetBodyName().ToString() + FString(TEXT("_")) + DataAssetClass.Get()->GetName();
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

#if !WITH_EDITOR
	if (ColorSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UColorSettings::StaticClass()->GetName())))
	{
		CreatePackageName(AssetName, PackagePath, *Outer, UColorSettings::StaticClass());
		ColorSettings = LoadObject<UColorSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ColorSettings == nullptr)
	{
		ColorSettings = Cast<UColorSettings>(CreateSettingsAsset(UColorSettings::StaticClass()));
		ColorSettings->GetPackage()->MarkPackageDirty();
	}

	if (ColorSettings->GetBiomeColorSettings() == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UBiomeColorSettings::StaticClass()->GetName())))
	{	
		CreatePackageName(AssetName, PackagePath, *Outer, UBiomeColorSettings::StaticClass());
		ColorSettings->SetBiomeColorSettings(LoadObject<UBiomeColorSettings>(Outer, *AssetName, *PackagePath));
	}
	else if (ColorSettings->GetBiomeColorSettings() == nullptr)
	{
		ColorSettings->SetBiomeColorSettings(Cast<UBiomeColorSettings>(CreateSettingsAsset(UBiomeColorSettings::StaticClass())));
		ColorSettings->GetBiomeColorSettings()->GetPackage()->MarkPackageDirty();
	}

	if ((ColorSettings->GetBiomeColorSettings()->GetBiomes() == TArray<UBiome*>() || ColorSettings->GetBiomeColorSettings()->GetBiomes()[0] == nullptr) && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UBiome::StaticClass()->GetName())))
	{
		ColorSettings->GetBiomeColorSettings()->GetBiomes().Empty();
		CreatePackageName(AssetName, PackagePath, *Outer, UBiome::StaticClass());
		ColorSettings->GetBiomeColorSettings()->GetBiomes().Add(LoadObject<UBiome>(Outer, *AssetName, *PackagePath));
	}
	else if (ColorSettings->GetBiomeColorSettings()->GetBiomes() == TArray<UBiome*>() || ColorSettings->GetBiomeColorSettings()->GetBiomes()[0] == nullptr)
	{
		ColorSettings->GetBiomeColorSettings()->GetBiomes().Empty();
		ColorSettings->GetBiomeColorSettings()->AddBiome(Cast<UBiome>(CreateSettingsAsset(UBiome::StaticClass())));
		ColorSettings->GetBiomeColorSettings()->GetBiomes()[0]->GetPackage()->MarkPackageDirty();
	}


	if (ShapeSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UShapeSettings::StaticClass()->GetName())))
	{
		CreatePackageName(AssetName, PackagePath, *Outer, UShapeSettings::StaticClass());
		ShapeSettings = LoadObject<UShapeSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ShapeSettings == nullptr)
	{
		ShapeSettings = Cast<UShapeSettings>(CreateSettingsAsset(UShapeSettings::StaticClass()));
		ShapeSettings->GetPackage()->MarkPackageDirty();
	}

	if ((ShapeSettings->GetNoiseLayers() == TArray<UNoiseLayer*>() || ShapeSettings->GetNoiseLayers()[0] == nullptr) && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UNoiseLayer::StaticClass()->GetName())))
	{
		ShapeSettings->GetNoiseLayers().Empty();
		CreatePackageName(AssetName, PackagePath, *Outer, UNoiseLayer::StaticClass());
		ShapeSettings->GetNoiseLayers().Add(LoadObject<UNoiseLayer>(Outer, *AssetName, *PackagePath));
	}
	else if (ShapeSettings->GetNoiseLayers() == TArray<UNoiseLayer*>() || ShapeSettings->GetNoiseLayers()[0] == nullptr)
	{
		ShapeSettings->GetNoiseLayers().Empty();
		ShapeSettings->AddNoiseLayer(Cast<UNoiseLayer>(CreateSettingsAsset(UNoiseLayer::StaticClass())));
		ShapeSettings->GetNoiseLayers()[0]->GetPackage()->MarkPackageDirty();
	}

	if (ShapeSettings->GetNoiseLayers()[0]->NoiseSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UNoiseSettings::StaticClass()->GetName())))
	{
		CreatePackageName(AssetName, PackagePath, *Outer, UNoiseSettings::StaticClass());
		ShapeSettings->GetNoiseLayers()[0]->NoiseSettings = LoadObject<UNoiseSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ShapeSettings->GetNoiseLayers()[0]->NoiseSettings == nullptr)
	{
		ShapeSettings->GetNoiseLayers()[0]->NoiseSettings = Cast<UNoiseSettings>(CreateSettingsAsset(UNoiseSettings::StaticClass()));
		ShapeSettings->GetNoiseLayers()[0]->NoiseSettings->GetPackage()->MarkPackageDirty();
	}
#else
	if (ColorSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UColorSettings::StaticClass()->GetName())))
	{
		CreatePackageName(AssetName, PackagePath, *Outer, UColorSettings::StaticClass());
		ColorSettings = LoadObject<UColorSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ColorSettings == nullptr)
	{
		ColorSettings = Cast<UColorSettings>(CreateSettingsAssetEditor(UColorSettings::StaticClass()));
		ColorSettings->GetPackage()->MarkPackageDirty();
	}

	if (ColorSettings->GetBiomeColorSettings() == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UBiomeColorSettings::StaticClass()->GetName())))
	{
		CreatePackageName(AssetName, PackagePath, *Outer, UBiomeColorSettings::StaticClass());
		ColorSettings->SetBiomeColorSettings(LoadObject<UBiomeColorSettings>(Outer, *AssetName, *PackagePath));
	}
	else if (ColorSettings->GetBiomeColorSettings() == nullptr)
	{
		ColorSettings->SetBiomeColorSettings(Cast<UBiomeColorSettings>(CreateSettingsAssetEditor(UBiomeColorSettings::StaticClass())));
		ColorSettings->GetBiomeColorSettings()->GetPackage()->MarkPackageDirty();
	}

	if ((ColorSettings->GetBiomeColorSettings()->GetBiomes() == TArray<UBiome*>() || ColorSettings->GetBiomeColorSettings()->GetBiomes()[0] == nullptr) && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UBiome::StaticClass()->GetName())))
	{
		ColorSettings->GetBiomeColorSettings()->GetBiomes().Empty();
		CreatePackageName(AssetName, PackagePath, *Outer, UBiome::StaticClass());
		ColorSettings->GetBiomeColorSettings()->AddBiome(LoadObject<UBiome>(Outer, *AssetName, *PackagePath));
	}
	else if (ColorSettings->GetBiomeColorSettings()->GetBiomes() == TArray<UBiome*>() || ColorSettings->GetBiomeColorSettings()->GetBiomes()[0] == nullptr)
	{
		ColorSettings->GetBiomeColorSettings()->GetBiomes().Empty();
		ColorSettings->GetBiomeColorSettings()->AddBiome(Cast<UBiome>(CreateSettingsAssetEditor(UBiome::StaticClass())));
		ColorSettings->GetBiomeColorSettings()->GetBiomes()[0]->GetPackage()->MarkPackageDirty();
	}


	if (ShapeSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UShapeSettings::StaticClass()->GetName())))
	{
		CreatePackageName(AssetName, PackagePath, *Outer, UShapeSettings::StaticClass());
		ShapeSettings = LoadObject<UShapeSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ShapeSettings == nullptr)
	{
		ShapeSettings = Cast<UShapeSettings>(CreateSettingsAssetEditor(UShapeSettings::StaticClass()));
		ShapeSettings->GetPackage()->MarkPackageDirty();
	}

	if ((ShapeSettings->GetNoiseLayers() == TArray<UNoiseLayer*>() || ShapeSettings->GetNoiseLayers()[0] == nullptr) && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UNoiseLayer::StaticClass()->GetName())))
	{
		ShapeSettings->GetNoiseLayers().Empty();
		CreatePackageName(AssetName, PackagePath, *Outer, UNoiseLayer::StaticClass());
		ShapeSettings->AddNoiseLayer(LoadObject<UNoiseLayer>(Outer, *AssetName, *PackagePath));
	}
	else if (ShapeSettings->GetNoiseLayers() == TArray<UNoiseLayer*>() || ShapeSettings->GetNoiseLayers()[0] == nullptr)
	{
		ShapeSettings->GetNoiseLayers().Empty();
		ShapeSettings->AddNoiseLayer(Cast<UNoiseLayer>(CreateSettingsAssetEditor(UNoiseLayer::StaticClass())));
		ShapeSettings->GetNoiseLayers()[0]->GetPackage()->MarkPackageDirty();
	}

	if (ShapeSettings->GetNoiseLayers()[0]->NoiseSettings == nullptr && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UNoiseSettings::StaticClass()->GetName())))
	{
		CreatePackageName(AssetName, PackagePath, *Outer, UNoiseSettings::StaticClass());
		ShapeSettings->GetNoiseLayers()[0]->NoiseSettings = LoadObject<UNoiseSettings>(Outer, *AssetName, *PackagePath);
	}
	else if (ShapeSettings->GetNoiseLayers()[0]->NoiseSettings == nullptr)
	{
		ShapeSettings->GetNoiseLayers()[0]->NoiseSettings = Cast<UNoiseSettings>(CreateSettingsAssetEditor(UNoiseSettings::StaticClass()));
		ShapeSettings->GetNoiseLayers()[0]->NoiseSettings->GetPackage()->MarkPackageDirty();
	}
#endif
	//AssetCleaner::CleanDirectory(EDirectoryFilterType::DataAssets);
}

void APlanet::GeneratePlanet()
{
	if (bGenerating == false)
	{
		bGenerating = true;
		UE_LOG(LogTemp, Warning, TEXT("%s started generating"), *BodyName.ToString());
		if (ColorSettings)
		{
			ProcMesh->SetRelativeLocation(FVector().ZeroVector);

			if (ShapeSettings != nullptr && ShapeSettings->IsNoiseLayers())
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
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *ShapeSettings->GetNoiseLayers()[0]->GetBodyName());
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
			if ((int)FaceRenderMask - 1 == i || FaceRenderMask == EFaceRenderMask::NoMask)
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
	if (bAutoGenerate)
	{
		Initialize();
		GenerateMesh();
	}
}

void APlanet::OnColorSettingsUpdated()
{
	if (bAutoGenerate)
	{
		for (int i = 0; i < 6; i++)
		{
			TerrainFaces[i]->UpdateBiomePercents();
		}
		GenerateColors();
	}
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
		
		if (bGenerating)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s finished generating"), *BodyName.ToString());
		}
		//AssetCleaner::CleanAll();
	}
	
	bGenerating = false;
	OnPlanetGenerated.ExecuteIfBound(this->BodyName);
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
