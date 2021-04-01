// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsteroidManager.generated.h"

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
	float e;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void NewVariants();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UTexture2D* CreateTexture(FString TextureName);

private:	
	UTexture2DArray* HeightmapsArray;

	TArray<UTexture2D*> Heightmaps;

	FVector PointOnSphere(FVector2D pointOnUnitSquare);

	// Returns true of the tested point is within the bounds of the face index i
	bool IsPointWithinFace(FVector2D pointToTest, int8 faceToTest);

};
