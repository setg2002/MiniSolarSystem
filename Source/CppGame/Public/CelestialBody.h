// This is a copyright notice

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CelestialGameMode.h"
#include "CelestialBody.generated.h"

UCLASS()
class CPPGAME_API ACelestialBody : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACelestialBody();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float mass;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float radius;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	FVector initialVelocity;

	UPROPERTY(Category = "Mesh", VisibleAnywhere, BlueprintReadOnly, meta = (AllowProvateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UFUNCTION(BlueprintCallable)
	void UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep);

	UFUNCTION(BlueprintCallable)
	void UpdatePosition(float timeStep);

	UPROPERTY(VisibleAnywhere)
	ACelestialGameMode* gameMode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	FVector currentVelocity;
};
