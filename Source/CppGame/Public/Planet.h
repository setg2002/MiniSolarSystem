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
class ShapeGenerator;
class TerrestrialColorGenerator;
class ShapeSettings;
class ColorSettings;



UENUM()
enum EFaceRenderMask { 
	All      UMETA(DisplayName = "All Faces"),
	Bottom   UMETA(DisplayName = "Bottom"),
	Top      UMETA(DisplayName = "Top"),
	Left     UMETA(DisplayName = "Left"),
	Right    UMETA(DisplayName = "Right"),
	Back     UMETA(DisplayName = "Back"),
	Front    UMETA(DisplayName = "Front")
};


UCLASS()
class CPPGAME_API APlanet : public ACelestialBody
{
	GENERATED_BODY()
	
public:
	APlanet();

	void ResetPosition();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Main mesh	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* ProcMesh;

	TerrainFace* TerrainFaces[6];

	ShapeGenerator* shapeGenerator;
	TerrestrialColorGenerator* colorGenerator;

	/* TODO Orbit stuff should be put in ACelestialBody instead */
	// The CelestialBody that orbitVelocity is to be calculated for
	UPROPERTY(Category = "Orbits", EditAnywhere)
	ACelestialBody* OrbitingBody;

	// The necessary velocity for this planet to orbit OrbitingBody
	UPROPERTY(Category = "Orbits", VisibleAnywhere)
	float orbitVelocity;

	// Sets orbitVelocity to the required magnitude to orbit OrbitingBody 
	UFUNCTION(Category = "Orbits", BlueprintCallable, CallInEditor)
	void CalculateOrbitVelocity();

	// Sets velocity to orbitVelocity
	UFUNCTION(Category = "Orbits", BlueprintCallable, CallInEditor)
	void SetToOrbit();

	// Shows/hides orbit calculation vectors
	UPROPERTY(Category = "Orbits", AdvancedDisplay, EditAnywhere, BlueprintReadWrite)
	bool bVectorDebug;

	UPROPERTY(Category = "Orbits", AdvancedDisplay, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bVectorDebug"))
	int32 VectorLength = 50;

	UPROPERTY(Category = "Orbits", AdvancedDisplay, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bVectorDebug"))
	float VectorSize = 1;

	UPROPERTY(Category = "Orbits", AdvancedDisplay, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bVectorDebug"))
	int VectorThickness = 1;

	UPROPERTY(Category = "Orbits", AdvancedDisplay, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bVectorDebug"))
	int32 VectorDuration = 10;

	// When true, the planet will call ReGenerate() every time a parameter is changed
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bAutoGenerate;

	// When true, planet terrain generation wil be multithreaded
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bMultithreadGeneration;

	// When true, tangents and normals will be generated for the planet mesh every time a parameter is changed (can only be true if bAutoGenerate is true)
	UPROPERTY(Category = "Settings", EditInstanceOnly, BlueprintReadWrite, meta = (EditCondition = "bAutoGenerate"))
	bool bAutoGenerateTangents;

	// Renders only the selected face
	UPROPERTY(Category = "Settings", EditInstanceOnly)
	TEnumAsByte<EFaceRenderMask> FaceRenderMask;

	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	class UColorSettings* ColorSettings;
	UPROPERTY(Category = "Settings", EditAnywhere, BlueprintReadWrite)
	class UShapeSettings* ShapeSettings;

	// Desired resolution for each TerrainFace to generate
	UPROPERTY(Category = "Mesh", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "2", ClampMax = "512"))
	int32 resolution = 16;

	// Initializes TerrainFaces
	void Initialize();

	// Makes new empty data asset for given data asset
	UDataAsset* CreateSettingsAsset(TSubclassOf<UDataAsset> DataAssetClass);

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

	UFUNCTION(Category = "Settings", BlueprintCallable, CallInEditor)
	void ReGenerateTangents();

	void GeneratePlanet();

	void GenerateMesh();

	void GenerateColors();

	void OnShapeSettingsUpdated();

	void OnColorSettingsUpdated();

	// Executes remaining funcetions necessary to complete planet generation once all terrain face threads are finished
	void ReconveneTerrainFaceThreads(int FaceNum);

	virtual void OnConstruction(const FTransform & Transform) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	virtual void PostEditMove(bool bFinished) override;
#endif

protected:
	virtual void BeginPlay() override;

	AOrbitDebugActor* OrbitDebugActor;

	void CreatePackageName(FString& OutAssetName, FString& OutPackagePath, UObject& OutOuter, TSubclassOf<UDataAsset> DataAssetClass);

	const FVector directions[6] = { FVector().UpVector,
									FVector().DownVector,
									FVector().LeftVector,
									FVector().RightVector,
									FVector().ForwardVector,
									FVector().BackwardVector };
};
