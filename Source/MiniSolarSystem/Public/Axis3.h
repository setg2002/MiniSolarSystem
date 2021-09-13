// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Axis3.generated.h"

class UBoxComponent;

UCLASS()
class MINISOLARSYSTEM_API AAxis3 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAxis3();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* BoxX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* BoxY;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* BoxZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test", meta = (ExposeOnSpawn = "true"))
	AActor* BodyToMove;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SwitchVisibility(uint8 Perspective) 
	{
		StaticMesh->SetVisibility(Perspective == 0 ? true : false);
	}

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// The axis that is currently selected. 0 is no axis and is the default, 1 is X-axis, 2 is Y-axis, and 3 is Z-axis
	UPROPERTY()
	uint8 Axis;

	uint8 bMouseDown;

	bool EnsureHit(bool BlockingHit, TWeakObjectPtr<AActor> HitActor, TWeakObjectPtr<UPrimitiveComponent> HitComponent);
};
