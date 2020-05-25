// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "CelestialGameMode.h"
#include "GameFramework/Actor.h"
#include "CelestialBody.generated.h"


//Forward Declarations
class ACelestialGameMode;

UCLASS()
class CPPGAME_API ACelestialBody : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACelestialBody();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = "Default")
	float mass;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float radius;
	
	UPROPERTY(EditInstanceOnly, Category = "Default")
	FVector initialVelocity;

	UPROPERTY(Category = "Mesh", VisibleAnywhere, BlueprintReadOnly, meta = (AllowProvateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UFUNCTION(BlueprintCallable)
	void UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep);

	UFUNCTION(BlueprintCallable)
	void UpdatePosition(float timeStep);

	ACelestialGameMode* gameMode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	FVector currentVelocity;
};
