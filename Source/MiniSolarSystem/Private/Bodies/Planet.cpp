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

	if (bGenerating && bMultithreadGeneration && !AreFacesGenerating())
	{
		if (bColorSettingsDirty)
		{
			// All workers are done, so the color generator and PointsOnUnitSphere are safe to touch.
			// OnColorSettingsUpdated finishes with GenerateColors(), which advances or completes generation
			bColorSettingsDirty = false;
			OnColorSettingsUpdated();
			return;
		}

		GenerateColors();
	}

}

bool APlanet::AreFacesGenerating() const
{
	for (TerrainFace* Face : TerrainFaces)
	{
		if (Face && !Face->GetIsFinished())
		{
			return true;
		}
	}
	return false;
}

void APlanet::ResetPosition()
{
	ProcMesh->SetRelativeLocation(FVector::ZeroVector);
}

UObject* APlanet::CreateSettingsAssetInDirectory(TSubclassOf<UObject> AssetClass, const FString& Directory, bool bSaveToDisk)
{
	// DA_<ID>_<Class>_<N>, using the first N that is neither a loaded package nor a package on disk. A package that is only on
	// disk must be skipped too: CreatePackage would give an empty, "partially loaded" package that SavePackage refuses to save
	const FString NamePrefix = FString::Printf(TEXT("DA_%u_%s_"), GetID(), *AssetClass->GetName());
	FString AssetName;
	FString PackagePath;
	int32 AssetNum = 0;
	do
	{
		AssetName = NamePrefix + FString::FromInt(AssetNum++);
		PackagePath = Directory + AssetName;
	}
	while (FindObject<UPackage>(nullptr, *PackagePath) != nullptr || FPackageName::DoesPackageExist(PackagePath));

	UPackage* Package = CreatePackage(*PackagePath);
	UObject* NewAsset = NewObject<UObject>(Package, AssetClass.Get(), *AssetName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);

	// Registered so the asset registry finds it (FindSettingsAsset(s)FromAppliedID, SaveGame's GetAssetsByPath) while it
	// only exists in memory
	FAssetRegistryModule::AssetCreated(NewAsset);

	if (!bSaveToDisk)
	{
		return NewAsset;
	}

	NewAsset->MarkPackageDirty();

	// SavePackage writes to disk, so map the package path (/Game/...) to the file in the project's Content folder
	const FString FilePath = FPackageName::LongPackageNameToFilename(PackagePath, FPackageName::GetAssetPackageExtension());
	FSavePackageArgs Args;
	Args.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
	Args.SaveFlags = SAVE_NoError; // Without this, a failed save goes to GError and crashes. The asset still works in memory
	const bool bSuccess = UPackage::SavePackage(Package, NewAsset, *FilePath, Args);
	UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), bSuccess ? TEXT("True") : TEXT("False"));

	return NewAsset;
}

FString APlanet::GetRuntimeSettingsAssetDirectory()
{
	return TEXT("/Game/DataAssets/Runtime/");
}

FString APlanet::GetEditorSettingsAssetDirectory() const
{
	return TEXT("/Game/DataAssets/") + BodyName.ToString() + TEXT("/");
}

UObject* APlanet::CreateSettingsAssetBP(TSubclassOf<UObject> AssetClass)
{
	return CreateSettingsAssetInDirectory(AssetClass, GetRuntimeSettingsAssetDirectory(), false);
}

UObject* APlanet::RestoreSettingsAsset(FName Name, TArray<uint8> Data, UClass* Class)
{
	// Runtime assets only live in memory and in the save game, so this is never saved to disk
	const FString PackageName = GetRuntimeSettingsAssetDirectory() + Name.ToString();

	// Reuse the asset if it is still in memory (e.g. loading a save twice in one session), overwriting its SaveGame data
	UObject* NewAsset = UMSSAssetFunctionLibrary::FindLoadedAsset(PackageName, Name, Class);
	if (!NewAsset)
	{
		NewAsset = NewObject<UObject>(CreatePackage(*PackageName), Class, Name, EObjectFlags::RF_Public);
	}

	if (NewAsset != NULL && Data.Num() > 1)
	{
		// Fill in the asset's data here
		FMemoryReader MemoryReader(Data);
		FCelestialSaveGameArchive Ar(MemoryReader);
		NewAsset->Serialize(Ar);
	}

	FAssetRegistryModule::AssetCreated(NewAsset);
	return NewAsset;
}

bool APlanet::UsesRuntimeSettingsAssets() const
{
	return !GIsEditor || (GWorld && GWorld->HasBegunPlay());
}

template< class T >
T* APlanet::CreateSettingsAssetForContext()
{
	return UsesRuntimeSettingsAssets() ? CreateSettingsAsset<T>(T::StaticClass())
	                                   : CreateSettingsAssetEditor<T>(T::StaticClass());
}

template< class T >
T* APlanet::FindOrCreateSettingsAsset(T* Existing, uint32 OwnerID)
{
	if (Existing)
	{
		return Existing;
	}
	if (T* Loaded = UMSSAssetFunctionLibrary::FindSettingsAssetFromAppliedID<T>(OwnerID))
	{
		return Loaded;
	}
	T* Created = CreateSettingsAssetForContext<T>();
	Created->AddAppliedID(OwnerID);
	return Created;
}

template< class T >
TArray<T*> APlanet::FindOrCreateSettingsAssets(uint32 OwnerID)
{
	TArray<T*> Found = UMSSAssetFunctionLibrary::FindSettingsAssetsFromAppliedID<T>(OwnerID);
	if (Found.IsEmpty())
	{
		T* Created = CreateSettingsAssetForContext<T>();
		Created->AddAppliedID(OwnerID);
		Found.Add(Created);
	}
	return Found;
}

// True when the array has no usable first entry, e.g. a freshly created settings asset or one whose references failed to load
template< class T >
static bool IsMissingFirstAsset(const TArray<T*>& Assets)
{
	return Assets.IsEmpty() || Assets[0] == nullptr;
}

void APlanet::ClearSettingsAssets()
{
	UE_LOG(LogTemp, Warning, TEXT("Cleared"));
	UnBindDelegates();
	if (ColorSettings)
	{
		if (ColorSettings->GetBiomeColorSettings())
		{
			ColorSettings->GetBiomeColorSettings()->RemoveAllBiomes();
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
		}
		ShapeSettings->RemoveAllNoiseLayers();
		ShapeSettings = nullptr;
	}
}

void APlanet::CreateSettingsAssets()
{
	// Color settings -> biome color settings -> biome noise + biomes
	ColorSettings = FindOrCreateSettingsAsset(ColorSettings, ID);

	// The setters broadcast OnSettingsAssetChanged, so they are only called when the value actually changes
	UBiomeColorSettings* BiomeSettings = FindOrCreateSettingsAsset(ColorSettings->GetBiomeColorSettings(), ColorSettings->GetID());
	if (ColorSettings->GetBiomeColorSettings() != BiomeSettings)
	{
		ColorSettings->SetBiomeColorSettings(BiomeSettings);
	}

	UNoiseSettings* BiomeNoise = FindOrCreateSettingsAsset(BiomeSettings->GetNoise(), BiomeSettings->GetID());
	if (BiomeSettings->GetNoise() != BiomeNoise)
	{
		BiomeSettings->SetNoise(BiomeNoise);
	}

	if (IsMissingFirstAsset(BiomeSettings->GetBiomes()))
	{
		BiomeSettings->RemoveAllBiomes();
		for (UBiome* Biome : FindOrCreateSettingsAssets<UBiome>(BiomeSettings->GetID()))
		{
			BiomeSettings->AddBiome(Biome);
		}
	}

	// Shape settings -> noise layers -> each layer's noise settings
	ShapeSettings = FindOrCreateSettingsAsset(ShapeSettings, ID);

	if (IsMissingFirstAsset(ShapeSettings->GetNoiseLayers()))
	{
		ShapeSettings->RemoveAllNoiseLayers();
		for (UNoiseLayer* Layer : FindOrCreateSettingsAssets<UNoiseLayer>(ShapeSettings->GetID()))
		{
			ShapeSettings->AddNoiseLayer(Layer);
		}
	}

	for (UNoiseLayer* Layer : ShapeSettings->GetNoiseLayers())
	{
		if (Layer)
		{
			Layer->NoiseSettings = FindOrCreateSettingsAsset(Layer->NoiseSettings, Layer->GetID());
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
	{
		OnPlanetGenerated.ExecuteIfBound(GetBodyName());
	}
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

void APlanet::Randomize()
{
	Super::Randomize();
	
	ShapeSettings->RandomizeForPlanet(this);
	ColorSettings->RandomizeForPlanet(this);
}

void APlanet::Initialize()
{
	// Make sure old terrain faces are canceled
	for (TerrainFace* Face : TerrainFaces)
	{
		if (Face)
		{
			Face->CancelTerrainFaceGeneration();
		}
	}
	
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
	if (!bAutoGenerate || TerrainFaces[0] == nullptr || gameMode->GetCurrentPerspective() == 128)
	{
		return;
	}
	
	if (AreFacesGenerating())
	{
		// Workers are reading ColorGenerator and writing PointsOnUnitSphere. Apply after they finish, without cancelling
		bColorSettingsDirty = true;
		return;
	}
	
	ColorGenerator->UpdateSettings(ColorSettings);
	for (int8 i = 0; i < 6; i++)
	{
		TerrainFaces[i]->UpdateBiomePercents();
	}
	
	GenerateColors();
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

void APlanet::SetRadius(float Radius)
{
	ShapeSettings->SetRadius(Radius);
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
		//TODO SG- Disabled for now as DrawOrbits uses the auth game mode to get bodies which is not available in editor when not PIE
		//AOrbitDebugActor::Get()->DrawOrbits();
	}
}
#endif
