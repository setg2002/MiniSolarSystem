// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "CelestialGameMode.h"
#include "TerrainFace.h"
#include "GameFramework/Actor.h"
#include "CelestialBody.generated.h"


//Forward Declarations
class ACelestialGameMode;
class TerrainFace;

UCLASS()
class CPPGAME_API ACelestialBody : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACelestialBody();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*Physics Interaction*/
	UPROPERTY(EditInstanceOnly, Category = "Default")
	float mass;

	UPROPERTY(EditInstanceOnly, Category = "Default")
	float radius;
	
	UPROPERTY(EditInstanceOnly, Category = "Default")
	FVector initialVelocity;

	UFUNCTION(BlueprintCallable)
	void UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep);

	UFUNCTION(BlueprintCallable)
	void UpdatePosition(float timeStep);

	ACelestialGameMode* gameMode;

	USceneComponent* Root;

	/*Mesh stuff*/
	TArray<UProceduralMeshComponent*> meshes;
	TerrainFace* terrainFaces[6];

	UPROPERTY(Category = "Mesh", EditAnywhere, meta = (ClampMin = "2", ClampMax = "256"))
	int resolution = 16;

	void Initialize();

	void GenerateMesh();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	FVector currentVelocity;
};
