// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "Planet.generated.h"

/**
 * 
 */

//Forward Declarations Here
class TerrainFace;
class UProceduralMeshComponent;
class AOrbitDebugActor;
class FShapeGenerator;
class FTerrestrialColorGenerator;
class UShapeSettings;
class UColorSettings;



UENUM()
enum EFaceRenderMask { 
	NoMask   UMETA(DisplayName = "All Faces"),
	Bottom   UMETA(DisplayName = "Bottom"),
	Top      UMETA(DisplayName = "Top"),
	Left     UMETA(DisplayName = "Left"),
	Right    UMETA(DisplayName = "Right"),
	Back     UMETA(DisplayName = "Back"),
	Front    UMETA(DisplayName = "Front")
};


UCLASS()
class MINISOLARSYSTEM_API APlanet : public ACelestialBody
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FPlanetGenerated, FName)
	
public:
	APlanet();
	~APlanet();

	FPlanetGenerated OnPlanetGenerated;
	
	// True when any face of the planet is still generating
	UFUNCTION(BlueprintCallable)
	bool AreFacesGenerating() const;

	void ResetPosition();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Main mesh	
	UPROPERTY()
	UProceduralMeshComponent* ProcMesh;

	TerrainFace* TerrainFaces[6];

	FShapeGenerator* ShapeGenerator;
	FTerrestrialColorGenerator* ColorGenerator;

	// When true, the planet will call ReGenerate() every time a parameter is changed
	UPROPERTY(SaveGame, Category = "Settings", EditAnywhere)
	bool bAutoGenerate = true;

	// When true, planet terrain generation wil be multithreaded
	UPROPERTY(SaveGame, Category = "Settings", EditAnywhere)
	bool bMultithreadGeneration = true;

	// When true, tangents and normals will be generated for the planet mesh every time a parameter is changed (can only be true if bAutoGenerate is true)
	UPROPERTY(Category = "Settings", EditInstanceOnly, BlueprintReadWrite, meta = (EditCondition = "bAutoGenerate"))
	bool bAutoGenerateTangents;

	// Renders only the selected face
	UPROPERTY(Category = "Settings", EditInstanceOnly)
	TEnumAsByte<EFaceRenderMask> FaceRenderMask;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	UColorSettings* ColorSettings;
	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	UShapeSettings* ShapeSettings;

	// Initializes TerrainFaces
	void Initialize();

	// Makes new empty data asset for given data asset, in the runtime folder (in memory only, like CreateSettingsAsset)
	UFUNCTION(BlueprintCallable)
	UObject* CreateSettingsAssetBP(TSubclassOf<UObject> AssetClass);
	// New in-memory asset in /Game/DataAssets/Runtime/. Never saved to disk: its data persists through the save game only
	template< class T >
	T* CreateSettingsAsset(TSubclassOf<UObject> AssetClass)
	{
		return CastChecked<T>(CreateSettingsAssetInDirectory(AssetClass, GetRuntimeSettingsAssetDirectory(), false));
	}
	// New asset in /Game/DataAssets/<BodyName>/, saved to disk
	template< class T >
	T* CreateSettingsAssetEditor(TSubclassOf<UObject> AssetClass)
	{
		return CastChecked<T>(CreateSettingsAssetInDirectory(AssetClass, GetEditorSettingsAssetDirectory(), true));
	}
	static UObject* RestoreSettingsAsset(FName Name, TArray<uint8> Data, UClass* Class);

	// Sets color and shape settings to null for regenerating planets from save i guess
	void ClearSettingsAssets();

	// Makes new blank data asset for shape and color settings if either are unassigned
	UFUNCTION(Category = "Settings", BlueprintCallable, CallInEditor)
	void CreateSettingsAssets();

	//TODO All of these Generate, ReGenerate, and On___Updated functions need to be sorted out

	UFUNCTION(Category = "Settings", BlueprintCallable, CallInEditor)
	void ClearMeshSections();

	// Blueprint callable for GeneratePlanet()
	UFUNCTION(Category = "Settings", BlueprintCallable, CallInEditor)
	void ReGenerate();

	UFUNCTION(Category = "Settings", BlueprintCallable, CallInEditor)
	void ReGenerateColors();
	
	virtual void Randomize() override;
	
	void GeneratePlanet();

	void GenerateMesh();

	void GenerateColors();

	UFUNCTION(BlueprintCallable)
	void OnShapeSettingsUpdated();
	UFUNCTION(BlueprintCallable)
	void OnColorSettingsUpdated();

	virtual void OnConstruction(const FTransform & Transform) override;

	UFUNCTION(BlueprintCallable)
	void BindDelegates();
	UFUNCTION(BlueprintCallable)
	void UnBindDelegates();
	UFUNCTION(BlueprintCallable)
	void BindSettingsIDs();

	virtual float GetBodyRadius() const override;
	
	void SetRadius(float Radius);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	virtual void PostEditMove(bool bFinished) override;
#endif

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// True from GeneratePlanet() until OnPlanetGenerated fires (spans all resolution levels)
	UPROPERTY(BlueprintReadOnly)
	bool bGenerating = false;
	
private:
	const FVector directions[6] = { FVector().UpVector,
									FVector().DownVector,
									FVector().LeftVector,
									FVector().RightVector,
									FVector().ForwardVector,
									FVector().BackwardVector };

	const int32 Resolutions[5] = { 16, 32, 64, 256, 1024 };
	int32 ResolutionLevel;
	
	uint64 GenerationTraceId;
	
	// Color settings changed while faces were generating. Will be applied when the current pass finishes
	bool bColorSettingsDirty = false;

	// Outside the editor, or once play has begun, settings assets go in /Game/DataAssets/Runtime/ instead of the body's editor folder
	bool UsesRuntimeSettingsAssets() const;

	// Creates and registers a new asset named DA_<ID>_<Class>_<N> in Directory (a long package path ending in '/'),
	// and saves it to disk if bSaveToDisk
	UObject* CreateSettingsAssetInDirectory(TSubclassOf<UObject> AssetClass, const FString& Directory, bool bSaveToDisk);
	static FString GetRuntimeSettingsAssetDirectory();
	FString GetEditorSettingsAssetDirectory() const;

	// CreateSettingsAsset or CreateSettingsAssetEditor, depending on UsesRuntimeSettingsAssets()
	template< class T >
	T* CreateSettingsAssetForContext();

	// Returns Existing if set, otherwise the asset applied to OwnerID, otherwise a new asset applied to OwnerID.
	// Only searches the asset registry when Existing is null
	template< class T >
	T* FindOrCreateSettingsAsset(T* Existing, uint32 OwnerID);

	// All assets applied to OwnerID, or a single new one applied to OwnerID if there are none
	template< class T >
	TArray<T*> FindOrCreateSettingsAssets(uint32 OwnerID);
};
