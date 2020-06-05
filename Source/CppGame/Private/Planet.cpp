// This is a copyright notice


#include "Planet.h"

APlanet::APlanet() 
{
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

void APlanet::Initialize()
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

void APlanet::GenerateMesh()
{
	for (auto& face : terrainFaces)
	{
		face->ConstructMesh();
	}
}

void APlanet::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, resolution))
		{
			Initialize();
			GenerateMesh();
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
