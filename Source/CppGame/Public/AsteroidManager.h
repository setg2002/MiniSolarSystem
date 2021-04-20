// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroidManager.generated.h"


class ShapeGenerator;
class UShapeSettings;

UCLASS()
class CPPGAME_API AAsteroidManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroidManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1", ClampMax = "10"))
	int32 NumVariants;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UShapeSettings* ShapeSettings;

	// Make new heightmaps
	UFUNCTION(BlueprintCallable, CallInEditor)
	void NewVariants();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	// Uses noise from shapeGenerator to generate a heightmap to be used as world displacement in the material
	UTexture2D* CreateHeightmapTexture(FString TextureName);

	// Creates a texture that represents the offset to apply to a cube to make it a sphere (quad-sphere style)
	UTexture2D* CreateSphereTexture(FString TextureName);
#endif

	ShapeGenerator* shapeGenerator;


private:	
	UTexture2DArray* HeightmapsArray;

	// Returns the point on a unit sphere projected onto the UVs of cube
	FVector PointOnUnitSphere(FVector2D pointOnUnitSquare);

	// Returns true of the tested point is within the coordinates of the index i
	bool IsPointWithinFace(FVector2D pointToTest, int8 faceToTest);

	const TArray<TArray<FVector2D>> Coordinates = {
	{ FVector2D(0, 0.25),    FVector2D(0.25, 0.5) },
	{ FVector2D(0.25, 0.5),  FVector2D(0.5, 0.75) },
	{ FVector2D(0.25, 0.25), FVector2D(0.5, 0.5)  },
	{ FVector2D(0.25, 0),    FVector2D(0.5, 0.25) },
	{ FVector2D(0.5, 0.25),  FVector2D(0.75, 0.5) },
	{ FVector2D(0.75, 0.25), FVector2D(1.f, .5)   }
	};

};
