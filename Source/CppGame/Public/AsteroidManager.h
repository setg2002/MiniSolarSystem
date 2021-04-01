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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 NumVariants;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UShapeSettings* ShapeSettings;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void NewVariants();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UTexture2D* CreateTexture(FString TextureName);

	ShapeGenerator* shapeGenerator;

private:	
	UTexture2DArray* HeightmapsArray;

	TArray<UTexture2D*> Heightmaps;

	FVector HeightAtPoint(FVector2D pointOnUnitSquare);

	// Returns true of the tested point is within the bounds of the face index i
	bool IsPointWithinFace(FVector2D pointToTest, int8 faceToTest);

	const TArray<TArray<FVector2D>> Coordinates = {
	{ FVector2D(0, 0.25),    FVector2D(0.25, 0.5) },
	{ FVector2D(0.25, 0.5),  FVector2D(0.5, 0.75) },
	{ FVector2D(0.25, 0.25), FVector2D(0.5, 0.5)  },
	{ FVector2D(0.25, 0),    FVector2D(0.5, 0.25) },
	{ FVector2D(0.5, 0.25),  FVector2D(0.75, 0.5) },
	{ FVector2D(0.75, 0.25), FVector2D(1.f, .5)   }
	};

	UStaticMesh* AsteroidMesh;

};
