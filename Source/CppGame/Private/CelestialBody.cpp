// This is a copyright notice


#include "CelestialBody.h"

// Sets default values
ACelestialBody::ACelestialBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	meshes = { CreateDefaultSubobject<UProceduralMeshComponent>("Mesh"), 
			   CreateDefaultSubobject<UProceduralMeshComponent>("Mesh1"), 
			   CreateDefaultSubobject<UProceduralMeshComponent>("Mesh2"), 
			   CreateDefaultSubobject<UProceduralMeshComponent>("Mesh3"),
			   CreateDefaultSubobject<UProceduralMeshComponent>("Mesh4"),
			   CreateDefaultSubobject<UProceduralMeshComponent>("Mesh5") 
	};

	Initialize();
	GenerateMesh();
}

void ACelestialBody::Initialize()
{
	FVector directions[6] = { FVector().UpVector, 
							  FVector().DownVector, 
							  FVector().LeftVector, 
							  FVector().RightVector, 
		                      FVector().ForwardVector, 
							  FVector().BackwardVector };

	for (int i = 0; i < 6; i++)
	{
		terrainFaces[i] = new TerrainFace(meshes[i], resolution, directions[i]);
	}
}

void ACelestialBody::GenerateMesh()
{
	for (auto& face : terrainFaces)
	{
		face->ConstructMesh();
	}
}

// Called when the game starts or when spawned
void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();

	currentVelocity = initialVelocity;
	gameMode = Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode());
}

void ACelestialBody::UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep)
{
	for(int i = 0; i < allBodies.Num(); i++)
	{
		ACelestialBody* otherBody = allBodies[i];

		if (otherBody != this && gameMode != nullptr) {
			float sqrDst = (otherBody->GetActorLocation() - this->GetActorLocation()).Size();
			FVector forceDir = (otherBody->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
			FVector force = forceDir * gameMode->gravitationalConstant * this->mass * otherBody->mass / sqrDst;
			FVector acceleration = force / this->mass;
			this->currentVelocity += acceleration * timeStep;
		}
	}
}

void ACelestialBody::UpdatePosition(float timeStep)
{
	this->SetActorLocation(FVector (this->GetActorLocation() += currentVelocity * timeStep));
}


// Called every frame
void ACelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

