// Copyright Soren Gilbertson


#include "Bodies/Planet.h"
#include "Shape/NoiseLayer.h"
#include "Bodies/TerrainFace.h"
#include "Shape/ShapeSettings.h"
#include "Color/ColorSettings.h"
#include "Color/ColorGenerator.h"
#include "Shape/ShapeGenerator.h"
#include "OrbitDebugActor.h"
#include "MeshDescription.h"
#include "Misc/PackageName.h"
#include "Engine/DataAsset.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Bodies/Components/AtmosphereComponent.h"
#include "UObject/PackageReload.h"
#include "Game/CelestialGameInstance.h"
#include "Game/CelestialGameMode.h"
#include "ProceduralMeshComponent.h"
#include "Game/CelestialSaveGameArchive.h"
#include "Helpers/SaveDataBlueprintFunctionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/SavePackage.h"


APlanet::APlanet()
{
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.bCanEverTick = true;

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh");
	ShapeGenerator = new FShapeGenerator();
	ColorGenerator = new FTerrestrialColorGenerator();
}

APlanet::~APlanet()
{
	delete ShapeGenerator;
	delete ColorGenerator;
}

void APlanet::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
}

void APlanet::BeginPlay()
{
	Super::BeginPlay();

	bGenerating = false;

#if WITH_EDITOR
	if (!ProcMesh) // Weird crash sometimes
		ProcMesh = NewObject<UProceduralMeshComponent>(this, FName("ProcMesh"));
#endif
	ensure(ProcMesh);
	ProcMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ProcMesh->SetRelativeLocation(FVector().ZeroVector);

	ResolutionLevel = 0;

}

void APlanet::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Stop threads, delete faces
	for (int8 i = 0; i < 6; i++)
	{
		if (TerrainFaces[i])
		{
			delete TerrainFaces[i];
			TerrainFaces[i] = nullptr;
		}
	}

	Super::EndPlay(EndPlayReason);
}

void APlanet::BindDelegates()
{
	UnBindDelegates();
	if (ShapeSettings)
	{
		ShapeSettings->OnSettingsAssetChanged.AddDynamic(this, &APlanet::OnShapeSettingsUpdated);
		if (ShapeSettings->IsNoiseLayers())
		{
			for (auto& NoiseLayer : ShapeSettings->GetNoiseLayers())
			{
				NoiseLayer->OnSettingsAssetChanged.AddDynamic(this, &APlanet::OnShapeSettingsUpdated);
				NoiseLayer->NoiseSettings->OnSettingsAssetChanged.AddDynamic(this, &APlanet::OnShapeSettingsUpdated);
			}
		}

	}
	if (ColorSettings)
	{
		ColorSettings->Init();
		ColorSettings->OnSettingsAssetChanged.AddDynamic(this, &APlanet::OnColorSettingsUpdated);
		if (ColorSettings->GetBiomeColorSettings())
		{
			ColorSettings->GetBiomeColorSettings()->OnSettingsAssetChanged.AddDynamic(this, &APlanet::OnColorSettingsUpdated);
			if (ColorSettings->GetBiomeColorSettings()->GetNoise())
			{
				ColorSettings->GetBiomeColorSettings()->GetNoise()->OnSettingsAssetChanged.AddDynamic(this, &APlanet::OnColorSettingsUpdated);
			}
			if (ColorSettings->GetBiomeColorSettings()->GetBiomes().Num() > 0)
			{
				for (auto& Biome : ColorSettings->GetBiomeColorSettings()->GetBiomes())
				{
					Biome->OnSettingsAssetChanged.AddDynamic(this, &APlanet::OnColorSettingsUpdated);
				}
			}
		}
	}
	BindSettingsIDs();
}

void APlanet::UnBindDelegates()
{
	if (ShapeSettings)
	{
		ShapeSettings->OnSettingsAssetChanged.RemoveAll(this);
		if (ShapeSettings->IsNoiseLayers())
		{
			for (auto& NoiseLayer : ShapeSettings->GetNoiseLayers())
			{
				NoiseLayer->OnSettingsAssetChanged.RemoveAll(this);
				NoiseLayer->NoiseSettings->OnSettingsAssetChanged.RemoveAll(this);
			}
		}

	}

	if (ColorSettings)
	{
		ColorSettings->OnSettingsAssetChanged.RemoveAll(this);
		if (ColorSettings->GetBiomeColorSettings())
		{
			ColorSettings->GetBiomeColorSettings()->OnSettingsAssetChanged.RemoveAll(this);
			if (ColorSettings->GetBiomeColorSettings()->GetNoise())
			{
				ColorSettings->GetBiomeColorSettings()->GetNoise()->OnSettingsAssetChanged.RemoveAll(this);
			}
			if (ColorSettings->GetBiomeColorSettings()->GetBiomes().Num() > 0)
			{
				for (auto& Biome : ColorSettings->GetBiomeColorSettings()->GetBiomes())
				{
					Biome->OnSettingsAssetChanged.RemoveAll(this);
				}
			}
		}
	}
}

void APlanet::BindSettingsIDs()
{
	if (ColorSettings)
		ColorSettings->AddAppliedID(ID);
	if (ColorSettings->GetBiomeColorSettings())
		ColorSettings->GetBiomeColorSettings()->AddAppliedID(ColorSettings->GetID());
	if (ColorSettings->GetBiomeColorSettings()->GetNoise())
	{
		ColorSettings->GetBiomeColorSettings()->GetNoise()->AddAppliedID(ColorSettings->GetBiomeColorSettings()->GetID());
		for (UBiome* Biome : ColorSettings->GetBiomeColorSettings()->GetBiomes())
		{
			Biome->AddAppliedID(ColorSettings->GetBiomeColorSettings()->GetID());
		}
	}

	if (ShapeSettings)
	{
		ShapeSettings->AddAppliedID(ID);
		for (UNoiseLayer* Layer : ShapeSettings->GetNoiseLayers())
		{
			Layer->AddAppliedID(ShapeSettings->GetID());
			if (Layer->NoiseSettings)
				Layer->NoiseSettings->AddAppliedID(Layer->GetID());
		}
	}
}

void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bGenerating && bMultithreadGeneration)
	{
		bool bAllThreadsFinished = true;
		for (int8 i = 0; i < 6; i++)
		{
			if (!TerrainFaces[i]->GetIsFinished())
			{
				bAllThreadsFinished = false;
				break;
			}
		}
		if (bAllThreadsFinished)
			GenerateColors();
	}

}

void APlanet::ResetPosition()
{
	ProcMesh->SetRelativeLocation(FVector::ZeroVector);
}

UObject* APlanet::CreateSettingsAssetBP(TSubclassOf<UObject> AssetClass)
{
	FString AssetPath = FString("/Game/DataAssets/Runtime/");
	FString AssetName = FString(TEXT("DA_")) + FString::FromInt(this->GetID()) + FString(TEXT("_")) + AssetClass.Get()->GetName() + "_0";
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

	UPackage* Package = CreatePackage(*PackagePath);
	UObject* NewAsset = NewObject<UObject>(Package, AssetClass.Get(), *AssetName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);

	FAssetRegistryModule::AssetCreated(NewAsset);
	NewAsset->MarkPackageDirty();

	FString FilePath = FString::Printf(TEXT("%s%s%s"), *AssetPath, *AssetName, *FPackageName::GetAssetPackageExtension());
	FSavePackageArgs Args = FSavePackageArgs();
	Args.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
	bool bSuccess = UPackage::SavePackage(Package, NewAsset, *FilePath, Args);
	UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), bSuccess ? TEXT("True") : TEXT("False"));

	return NewAsset;
}

template< class T >
T* APlanet::CreateSettingsAsset(TSubclassOf<UObject> AssetClass)
{
	FString AssetPath = FString("/Game/DataAssets/Runtime/");
	FString AssetName = FString(TEXT("DA_")) + FString::FromInt(this->GetID()) + FString(TEXT("_")) + AssetClass.Get()->GetName() + "_0";
	FString PackagePath = AssetPath + AssetName;
	
	int AssetNum = 0;
	bool PackageExists = FindObject<UPackage>(nullptr, *PackagePath) == NULL ? false : true;
	while (PackageExists)
	{
		AssetNum++;
		AssetName = FString(TEXT("DA_")) + FString::FromInt(this->GetID()) + FString(TEXT("_")) + AssetClass.Get()->GetName() + "_" + FString::FromInt(AssetNum);
		PackagePath = AssetPath + AssetName;

		PackageExists = FindObject<UPackage>(nullptr, *PackagePath) == NULL ? false : true;
	}

	UPackage *Package = CreatePackage(*PackagePath);
	T* NewAsset = NewObject<T>(Package, AssetClass.Get(), *AssetName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);

	FAssetRegistryModule::AssetCreated(NewAsset);
	NewAsset->MarkPackageDirty();

	FString FilePath = FString::Printf(TEXT("%s%s%s"), *AssetPath, *AssetName, *FPackageName::GetAssetPackageExtension());
	FSavePackageArgs Args = FSavePackageArgs();
	Args.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
	bool bSuccess = UPackage::SavePackage(Package, NewAsset, *FilePath, Args);
	UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), bSuccess ? TEXT("True") : TEXT("False"));

	return NewAsset;
}

template< class T >
T* APlanet::CreateSettingsAssetEditor(TSubclassOf<UObject> AssetClass)
{
	FString AssetPath = FString("/Game/DataAssets/" + this->BodyName.ToString() + "/");
	FString AssetName = FString(TEXT("DA_")) + FString::FromInt(this->GetID()) + FString(TEXT("_")) + AssetClass.Get()->GetName() + "_0";
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
	T* NewAsset = NewObject<T>(Package, AssetClass.Get(), *AssetName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);

	FAssetRegistryModule::AssetCreated(NewAsset);
	NewAsset->MarkPackageDirty();

	FString FilePath = FString::Printf(TEXT("%s%s%s"), *AssetPath, *AssetName, *FPackageName::GetAssetPackageExtension());
	FSavePackageArgs Args = FSavePackageArgs();
	Args.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
	bool bSuccess = UPackage::SavePackage(Package, NewAsset, *FilePath, Args);
	UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), bSuccess ? TEXT("True") : TEXT("False"));

	return NewAsset;
}

UObject* APlanet::RestoreSettingsAsset(FName Name, TArray<uint8> Data, UClass* Class)
{
	FString PackageName = TEXT("/Game/DataAssets/Runtime/" + Name.ToString());
	UPackage* Package = CreatePackage(*PackageName);

	UObject* NewAsset = NewObject<UObject>(Package, Class, Name, EObjectFlags::RF_Public);

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
	FSavePackageArgs Args = FSavePackageArgs();
	Args.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
	bool bSuccess = UPackage::SavePackage(Package, NewAsset, *FilePath, Args);
	return NewAsset;
}

void APlanet::CreatePackageName(FString& OutAssetName, FString& OutPackagePath, UObject& OutOuter, TSubclassOf<UObject> DataAssetClass)
{
	FString AssetPath = FString("/Game/DataAssets/" + FString::FromInt(this->GetID()) + "/");
	OutAssetName = FString(TEXT("DA_")) + FString::FromInt(this->GetID()) + FString(TEXT("_")) + DataAssetClass->GetName();
	OutPackagePath = AssetPath + OutAssetName;
	UObject* OuterPtr = &OutOuter;
	OuterPtr = FindPackage(this, *OutPackagePath);
}

void APlanet::ClearSettingsAssets()
{
	UE_LOG(LogTemp, Warning, TEXT("Cleared"));
	UnBindDelegates();
	if (ColorSettings)
	{
		if (ColorSettings->GetBiomeColorSettings())
		{
			for (UBiome* biome : ColorSettings->GetBiomeColorSettings()->GetBiomes())
			{
				ColorSettings->GetBiomeColorSettings()->RemoveBiomeByRef(biome);
			}
			ColorSettings->SetBiomeColorSettings(nullptr);
		}
		ColorSettings = nullptr;
	}
	if (ShapeSettings)
	{
		for (UNoiseLayer* Layer : ShapeSettings->GetNoiseLayers())
		{
			if (Layer)
			{
				Layer->NoiseSettings = nullptr;
			}
			ShapeSettings->RemoveNoiseLayer(ShapeSettings->GetNoiseLayers().Num() - 1);
		}
		ShapeSettings = nullptr;
	}
}

void APlanet::CreateSettingsAssets()
{
	FString AssetName;
	FString PackagePath;
	UObject* Outer = nullptr;

	if (!GIsEditor || GWorld->HasBegunPlay())
	{ 
		UColorSettings* LoadedColorSettings = UMSSAssetFunctionLibrary::FindSettingsAssetFromAppliedID<UColorSettings>(ID);
		if (ColorSettings == nullptr && LoadedColorSettings)
		{
			ColorSettings = LoadedColorSettings;
		}
		else if (ColorSettings == nullptr)
		{
			ColorSettings = CreateSettingsAsset<UColorSettings>(UColorSettings::StaticClass());
			ColorSettings->GetPackage()->MarkPackageDirty();
			ColorSettings->AddAppliedID(ID);
		}

		UBiomeColorSettings* LoadedBiomeColorSettings = UMSSAssetFunctionLibrary::FindSettingsAssetFromAppliedID<UBiomeColorSettings>(ColorSettings->GetID());
		if (ColorSettings->GetBiomeColorSettings() == nullptr && LoadedBiomeColorSettings)
		{	
			ColorSettings->SetBiomeColorSettings(LoadedBiomeColorSettings);
		}
		else if (ColorSettings->GetBiomeColorSettings() == nullptr)
		{
			ColorSettings->SetBiomeColorSettings(CreateSettingsAsset<UBiomeColorSettings>(UBiomeColorSettings::StaticClass()));
			ColorSettings->GetBiomeColorSettings()->GetPackage()->MarkPackageDirty();
			ColorSettings->GetBiomeColorSettings()->AddAppliedID(ColorSettings->GetID());
		}

		UNoiseSettings* LoadedBiomeNoiseSettings = UMSSAssetFunctionLibrary::FindSettingsAssetFromAppliedID<UNoiseSettings>(ColorSettings->GetBiomeColorSettings()->GetID());
		if (ColorSettings->GetBiomeColorSettings()->GetNoise() == nullptr && LoadedBiomeNoiseSettings)
		{
			ColorSettings->GetBiomeColorSettings()->SetNoise(LoadedBiomeNoiseSettings);
		}
		else if (ColorSettings->GetBiomeColorSettings()->GetNoise() == nullptr)
		{
			ColorSettings->GetBiomeColorSettings()->SetNoise(CreateSettingsAsset<UNoiseSettings>(UNoiseSettings::StaticClass()));
			ColorSettings->GetBiomeColorSettings()->GetNoise()->GetPackage()->MarkPackageDirty();
			ColorSettings->GetBiomeColorSettings()->GetNoise()->AddAppliedID(ColorSettings->GetBiomeColorSettings()->GetID());
		}

		TArray<UBiome*> LoadedBiomeAssets = UMSSAssetFunctionLibrary::FindSettingsAssetsFromAppliedID<UBiome>(ColorSettings->GetBiomeColorSettings()->GetID());
		if ((ColorSettings->GetBiomeColorSettings()->GetBiomes() == TArray<UBiome*>() || ColorSettings->GetBiomeColorSettings()->GetBiomes()[0] == nullptr) && LoadedBiomeAssets.Num() > 0)
		{
			ColorSettings->GetBiomeColorSettings()->GetBiomes().Empty();
			for (UBiome* LoadedBiomeAsset : LoadedBiomeAssets)
			{
				ColorSettings->GetBiomeColorSettings()->AddBiome(LoadedBiomeAsset);
			}
		}
		else if (ColorSettings->GetBiomeColorSettings()->GetBiomes() == TArray<UBiome*>() || ColorSettings->GetBiomeColorSettings()->GetBiomes()[0] == nullptr)
		{
			ColorSettings->GetBiomeColorSettings()->GetBiomes().Empty();
			ColorSettings->GetBiomeColorSettings()->AddBiome(CreateSettingsAsset<UBiome>(UBiome::StaticClass()));
			ColorSettings->GetBiomeColorSettings()->GetBiomes()[0]->GetPackage()->MarkPackageDirty();
			ColorSettings->GetBiomeColorSettings()->GetBiomes()[0]->AddAppliedID(ColorSettings->GetBiomeColorSettings()->GetID());
		}


		UShapeSettings* LoadedShapeSettings = UMSSAssetFunctionLibrary::FindSettingsAssetFromAppliedID<UShapeSettings>(ID);
		if (ShapeSettings == nullptr && LoadedShapeSettings)
		{
			ShapeSettings = LoadedShapeSettings;
		}
		else if (ShapeSettings == nullptr)
		{
			ShapeSettings = CreateSettingsAsset<UShapeSettings>(UShapeSettings::StaticClass());
			ShapeSettings->GetPackage()->MarkPackageDirty();
			ShapeSettings->AddAppliedID(ID);
		}

		TArray<UNoiseLayer*> LoadedShapeNoiseLayers = UMSSAssetFunctionLibrary::FindSettingsAssetsFromAppliedID<UNoiseLayer>(ShapeSettings->GetID());
		if ((ShapeSettings->GetNoiseLayers() == TArray<UNoiseLayer*>() || ShapeSettings->GetNoiseLayers()[0] == nullptr) && FPackageName::DoesPackageExist(FString("/Game/DataAssets/" + this->BodyName.ToString() + "/" + "DA_" + this->BodyName.ToString() + "_" + UNoiseLayer::StaticClass()->GetName())))
		{
			ShapeSettings->GetNoiseLayers().Empty();
			CreatePackageName(AssetName, PackagePath, *Outer, UNoiseLayer::StaticClass());
			ShapeSettings->GetNoiseLayers().Add(LoadObject<UNoiseLayer>(Outer, *AssetName, *PackagePath));
			ShapeSettings->GetNoiseLayers().Last()->AddAppliedID(ShapeSettings->GetID());
		}
		else if (ShapeSettings->GetNoiseLayers() == TArray<UNoiseLayer*>() || ShapeSettings->GetNoiseLayers()[0] == nullptr)
		{
			ShapeSettings->GetNoiseLayers().Empty();
			ShapeSettings->AddNoiseLayer(CreateSettingsAsset<UNoiseLayer>(UNoiseLayer::StaticClass()));
			ShapeSettings->GetNoiseLayers()[0]->GetPackage()->MarkPackageDirty();
			ShapeSettings->GetNoiseLayers()[0]->AddAppliedID(ShapeSettings->GetID());
		}

		UNoiseSettings* LoadedFirstNoiseLayerNoiseSettings = UMSSAssetFunctionLibrary::FindSettingsAssetFromAppliedID<UNoiseSettings>(ShapeSettings->GetNoiseLayers()[0]->GetID());
		if (ShapeSettings->GetNoiseLayers()[0]->NoiseSettings == nullptr && LoadedFirstNoiseLayerNoiseSettings)
		{
			ShapeSettings->GetNoiseLayers()[0]->NoiseSettings = LoadedFirstNoiseLayerNoiseSettings;
		}
		else if (ShapeSettings->GetNoiseLayers()[0]->NoiseSettings == nullptr)
		{
			ShapeSettings->GetNoiseLayers()[0]->NoiseSettings = CreateSettingsAsset<UNoiseSettings>(UNoiseSettings::StaticClass());
			ShapeSettings->GetNoiseLayers()[0]->NoiseSettings->GetPackage()->MarkPackageDirty();
			ShapeSettings->GetNoiseLayers()[0]->NoiseSettings->AddAppliedID(ShapeSettings->GetNoiseLayers()[0]->GetID());
		}
	}
	else
	{
		UColorSettings* LoadedColorSettings = UMSSAssetFunctionLibrary::FindSettingsAssetFromAppliedID<UColorSettings>(ID);
		if (ColorSettings == nullptr && LoadedColorSettings)
		{
			ColorSettings = LoadedColorSettings;
		}
		else if (ColorSettings == nullptr)
		{
			ColorSettings = CreateSettingsAssetEditor<UColorSettings>(UColorSettings::StaticClass());
			ColorSettings->GetPackage()->MarkPackageDirty();
			ColorSettings->AddAppliedID(ID);
		}

		UBiomeColorSettings* LoadedBiomeColorSettings = UMSSAssetFunctionLibrary::FindSettingsAssetFromAppliedID<UBiomeColorSettings>(ColorSettings->GetID());
		if (ColorSettings->GetBiomeColorSettings() == nullptr && LoadedBiomeColorSettings)
		{
			ColorSettings->SetBiomeColorSettings(LoadedBiomeColorSettings);
		}
		else if (ColorSettings->GetBiomeColorSettings() == nullptr)
		{
			ColorSettings->SetBiomeColorSettings(CreateSettingsAssetEditor<UBiomeColorSettings>(UBiomeColorSettings::StaticClass()));
			ColorSettings->GetBiomeColorSettings()->GetPackage()->MarkPackageDirty();
			ColorSettings->GetBiomeColorSettings()->AddAppliedID(ColorSettings->GetID());
		}

		UNoiseSettings* LoadedBiomeNoiseSettings = UMSSAssetFunctionLibrary::FindSettingsAssetFromAppliedID<UNoiseSettings>(ColorSettings->GetBiomeColorSettings()->GetID());
		if (ColorSettings->GetBiomeColorSettings()->GetNoise() == nullptr && LoadedBiomeNoiseSettings)
		{
			ColorSettings->GetBiomeColorSettings()->SetNoise(LoadedBiomeNoiseSettings);
		}
		else if (ColorSettings->GetBiomeColorSettings()->GetNoise() == nullptr)
		{
			ColorSettings->GetBiomeColorSettings()->SetNoise(CreateSettingsAssetEditor<UNoiseSettings>(UNoiseSettings::StaticClass()));
			ColorSettings->GetBiomeColorSettings()->GetNoise()->GetPackage()->MarkPackageDirty();
			ColorSettings->GetBiomeColorSettings()->GetNoise()->AddAppliedID(ColorSettings->GetBiomeColorSettings()->GetID());
		}

		TArray<UBiome*> LoadedBiomes = UMSSAssetFunctionLibrary::FindSettingsAssetsFromAppliedID<UBiome>(ColorSettings->GetBiomeColorSettings()->GetID());
		if ((ColorSettings->GetBiomeColorSettings()->GetBiomes() == TArray<UBiome*>() || ColorSettings->GetBiomeColorSettings()->GetBiomes()[0] == nullptr) && LoadedBiomes.Num() > 0)
		{
			ColorSettings->GetBiomeColorSettings()->GetBiomes().Empty();
			for (UBiome* LoadedBiome : LoadedBiomes)
			{
				ColorSettings->GetBiomeColorSettings()->AddBiome(LoadedBiome);	
			}
		}
		else if (ColorSettings->GetBiomeColorSettings()->GetBiomes() == TArray<UBiome*>() || ColorSettings->GetBiomeColorSettings()->GetBiomes()[0] == nullptr)
		{
			ColorSettings->GetBiomeColorSettings()->GetBiomes().Empty();
			ColorSettings->GetBiomeColorSettings()->AddBiome(CreateSettingsAssetEditor<UBiome>(UBiome::StaticClass()));
			ColorSettings->GetBiomeColorSettings()->GetBiomes()[0]->GetPackage()->MarkPackageDirty();
			ColorSettings->GetBiomeColorSettings()->GetBiomes()[0]->AddAppliedID(ColorSettings->GetBiomeColorSettings()->GetID());
		}


		UShapeSettings* LoadedShapeSettings = UMSSAssetFunctionLibrary::FindSettingsAssetFromAppliedID<UShapeSettings>(ID);
		if (ShapeSettings == nullptr && LoadedShapeSettings)
		{
			ShapeSettings = LoadedShapeSettings;
		}
		else if (ShapeSettings == nullptr)
		{
			ShapeSettings = CreateSettingsAssetEditor<UShapeSettings>(UShapeSettings::StaticClass());
			ShapeSettings->GetPackage()->MarkPackageDirty();
			ShapeSettings->AddAppliedID(ID);
		}

		TArray<UNoiseLayer*> LoadedShapeNoiseLayers = UMSSAssetFunctionLibrary::FindSettingsAssetsFromAppliedID<UNoiseLayer>(ShapeSettings->GetID());
		if ((ShapeSettings->GetNoiseLayers() == TArray<UNoiseLayer*>() || ShapeSettings->GetNoiseLayers()[0] == nullptr) && LoadedShapeNoiseLayers.Num() > 0)
		{
			ShapeSettings->GetNoiseLayers().Empty();
			for (UNoiseLayer* LoadedNoiseLayer : LoadedShapeNoiseLayers)
			{
				ShapeSettings->AddNoiseLayer(LoadedNoiseLayer);
			}
		}
		else if (ShapeSettings->GetNoiseLayers() == TArray<UNoiseLayer*>() || ShapeSettings->GetNoiseLayers()[0] == nullptr)
		{
			ShapeSettings->GetNoiseLayers().Empty();
			ShapeSettings->AddNoiseLayer(CreateSettingsAssetEditor<UNoiseLayer>(UNoiseLayer::StaticClass()));
			ShapeSettings->GetNoiseLayers()[0]->GetPackage()->MarkPackageDirty();
			ShapeSettings->GetNoiseLayers()[0]->AddAppliedID(ShapeSettings->GetID());
		}

		UNoiseSettings* LoadedFirstNoiseLayerNoiseSettings = UMSSAssetFunctionLibrary::FindSettingsAssetFromAppliedID<UNoiseSettings>(ShapeSettings->GetNoiseLayers()[0]->GetID());
		if (ShapeSettings->GetNoiseLayers()[0]->NoiseSettings == nullptr && LoadedFirstNoiseLayerNoiseSettings)
		{
			ShapeSettings->GetNoiseLayers()[0]->NoiseSettings = LoadedFirstNoiseLayerNoiseSettings;
		}
		else if (ShapeSettings->GetNoiseLayers()[0]->NoiseSettings == nullptr)
		{
			ShapeSettings->GetNoiseLayers()[0]->NoiseSettings = CreateSettingsAssetEditor<UNoiseSettings>(UNoiseSettings::StaticClass());
			ShapeSettings->GetNoiseLayers()[0]->NoiseSettings->GetPackage()->MarkPackageDirty();
			ShapeSettings->GetNoiseLayers()[0]->NoiseSettings->AddAppliedID(ShapeSettings->GetNoiseLayers()[0]->GetID());
		}
	}
}

void APlanet::GeneratePlanet()
{
	if (!bGenerating && ShapeSettings && ColorSettings)
	{
		GenerationTraceId = TRACE_BEGIN_REGION_WITH_ID(*FString::Printf(TEXT("Generate Planet: %s"), *BodyName.ToString()), TEXT("Planets"))
		
		if (ResolutionLevel == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s started generating"), *BodyName.ToString());
			Collider->SetSphereRadius(ShapeSettings->GetRadius() + 10);
		}

		if (ShapeSettings->IsNoiseLayers())
		{
			bGenerating = true;
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
	else
		OnPlanetGenerated.ExecuteIfBound(GetBodyName());
}

void APlanet::ClearMeshSections()
{
	for (int8 i = 0; i < 6; i++)
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

void APlanet::Initialize()
{
	BindSettingsIDs();
	ShapeGenerator->UpdateSettings(ShapeSettings);
	ColorGenerator->UpdateSettings(ColorSettings);
	
	ResolutionLevel = Cast<UCelestialGameInstance>(GetGameInstance())->GetResMax() - 1;

	if (!ProcMesh->GetMaterial(0))
		ColorSettings->DynamicMaterial = UMaterialInstanceDynamic::Create(ColorSettings->PlanetMat, this);
	
	for (int8 i = 0; i < 6; i++)
	{
		if (!TerrainFaces[i])
			TerrainFaces[i] = new TerrainFace(i, ShapeGenerator, ColorGenerator, Resolutions[ResolutionLevel], directions[i], ProcMesh);
		else
			TerrainFaces[i]->UpdateResolution(Resolutions[ResolutionLevel]);
			
		if (!ProcMesh->GetMaterial(i))
			ProcMesh->SetMaterial(i, ColorSettings->DynamicMaterial);
	}
}

void APlanet::GenerateMesh()
{
	if (bMultithreadGeneration)
	{
		for (int8 i = 0; i < 6; i++)
		{
			TerrainFaces[i]->ConstructMeshAsync();
		}
	}
	else 
	{
		for (int8 i = 0; i < 6; i++)
		{
			if ((int)FaceRenderMask - 1 == i || FaceRenderMask == EFaceRenderMask::NoMask)
				TerrainFaces[i]->CalculateMesh();
			else
				ProcMesh->ClearMeshSection(i);
		}
		ColorGenerator->UpdateElevation(ShapeGenerator->ElevationMinMax);
	}
}

void APlanet::OnShapeSettingsUpdated()
{
	if (bAutoGenerate && ShapeSettings && ColorSettings && gameMode->GetCurrentPerspective() != 128)
	{
		if (bGenerating && ResolutionLevel != 0)
		{
			ResolutionLevel = 0;
			bGenerating = false;
			GeneratePlanet();
		}
		else
			GeneratePlanet();

		// Update the size of the atmosphere
		for (UActorComponent* Component : GetComponents().Array())
		{
			if (Component->GetClass() == UAtmosphereComponent::StaticClass())
				Cast<UAtmosphereComponent>(Component)->SetRelativeScale3D(FVector(ShapeSettings->GetRadius() * 0.0125f));
		}
	}
}

void APlanet::OnColorSettingsUpdated()
{
	if (bAutoGenerate && TerrainFaces[0] != NULL && gameMode->GetCurrentPerspective() != 128)
	{
		ColorGenerator->UpdateSettings(ColorSettings);
		for (int8 i = 0; i < 6; i++)
		{
			TerrainFaces[i]->UpdateBiomePercents();
		}
		GenerateColors();
	}
}

void APlanet::GenerateColors()
{
	ColorGenerator->UpdateColors();

	// Reload the texture
	ProcMesh->GetMaterial(0)->LoadConfig();

	if (bMultithreadGeneration)
		ColorGenerator->UpdateElevation(ShapeGenerator->ElevationMinMax);

	if (GetWorld() && bGenerating)
	{
		bGenerating = false;

		if (ResolutionLevel < Cast<UCelestialGameInstance>(GetGameInstance())->GetResMax() - 1) // Regenerate at the next level
		{
			ResolutionLevel++;
			ReGenerate();
		}
		else
		{	
			ResolutionLevel = 0;
			UE_LOG(LogTemp, Warning, TEXT("%s finished generating"), *BodyName.ToString());
			OnPlanetGenerated.ExecuteIfBound(this->BodyName);
			TRACE_END_REGION_WITH_ID(GenerationTraceId);
		}
	}	
}

float APlanet::GetBodyRadius() const
{
	return ShapeSettings->GetRadius();
}

#if WITH_EDITOR
void APlanet::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		/*if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, ShapeSettings) && bAutoGenerate)
		{
			OnShapeSettingsUpdated();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, ColorSettings) && bAutoGenerate)
		{
			OnColorSettingsUpdated();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, FaceRenderMask) && bAutoGenerate)
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
