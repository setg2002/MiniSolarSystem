// This is a copyright notice

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

	/*Mesh stuff*/	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;
	// Procedural meshes
	TArray<UProceduralMeshComponent*> ProcMeshes; //TODO Destroy proc meshes when not in use

	TerrainFace* TerrainFaces[6];

	ShapeGenerator* shapeGenerator;
	TerrestrialColorGenerator* colorGenerator;

	// Converts all ProcMeshes into a single static mesh that can be more easily saved to disk
	UStaticMesh* ConvertToStaticMesh();

	/* TODO Orbit stuff should be put in ACelestialBody instead */
	// The CelestialBody that orbitVelocity is to be calculated for
	UPROPERTY(Category = "Orbits", EditAnywhere)
	ACelestialBody* OrbitingBody;

	UPROPERTY(Category = "Orbits", EditAnywhere, meta = (ClampMin = "0"))
	float m = 0.5f; //0.4586f;

	UPROPERTY(Category = "Orbits", EditAnywhere, meta = (ClampMin = "0"))
	float b = 50.5f;

	// The necessary velocity for this planet to orbit OrbitingBody
	UPROPERTY(Category = "Orbits", VisibleAnywhere)
	float orbitVelocity;

	// Sets orbitVelocity to the required magnitude to orbit OrbitingBody 
	/* 
		This calculation is really janky rn and is based off really approximate trendlines
	so I would not recommend using it. It also doesn't take the gravitational constant (G) into 
	account	and so only "works" with G = 100. 
	*/
	UFUNCTION(Category = "Orbits", BlueprintCallable, CallInEditor)
	void CalculateOrbitVelocity(); //TODO Make more reliable

	// Sets velocity to orbitVelocity
	UFUNCTION(Category = "Orbits", BlueprintCallable, CallInEditor)
	void SetToOrbit();

	/* Each element in the array represents the status of a TerrainFace thread. The entire array is set to false
	in GeneratePlanet(), when terrain generation starts. ConvertAndSetStaticMesh(int32 i) then uses the array to 
	validate that all threads have finished before creating teh static mesh from the procedural meshes.        */
	bool ThreadsFinished[6];

	// When true, the planet will call ReGenerate() every time a parameter is changed
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bAutoGenerate;

	// When true, planet terrain generation wil be multithreaded
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool bMultithreadGeneration;

	// When true, tangents and normals will be generated for the planet mesh every time a parameter is changed (can only be true if bAutoGenerate is true)
	UPROPERTY(Category = "Settings", EditInstanceOnly, BlueprintReadWrite, meta = (EditCondition = "bAutoGenerate"))
	bool bAutoGenerateTangents;

	// Not currently working
	UPROPERTY(Category = "Settings", EditInstanceOnly)
	TEnumAsByte<EFaceRenderMask> FaceRenderMask;

	UPROPERTY(Category = "Settings", EditAnywhere)
	class UColorSettings* ColorSettings;
	UPROPERTY(Category = "Settings", EditAnywhere)
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

	// Converts procedural meshes of ProcMeshes array to static meshes then combines all of them and assigns the resulting mesh to StaticMesh
	void ConvertAndSetStaticMesh(int32 i);

	virtual void OnConstruction(const FTransform & Transform) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	AOrbitDebugActor* OrbitDebugActor;

	void CreatePackageName(FString& OutAssetName, FString& OutPackagePath, UObject& OutOuter, TSubclassOf<UDataAsset> DataAssetClass);

	const FVector directions[6] = { FVector().UpVector,
									FVector().DownVector,
									FVector().LeftVector,
									FVector().RightVector,
									FVector().ForwardVector,
									FVector().BackwardVector };
};
