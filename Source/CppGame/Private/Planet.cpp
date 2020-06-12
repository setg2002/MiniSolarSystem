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

	//Sets material
	for (auto& mesh : meshes)
	{
		mesh->SetMaterial(0, PlanetMat);
	}

	GeneratePlanet();
}

void APlanet::GeneratePlanet()
{
	if (ShapeSettings != nullptr)
	{
		Initialize();
		GenerateMesh();
		GenerateColors();
	}
}

void APlanet::Initialize()
{
	shapeGenerator = new ShapeGenerator(ShapeSettings);

	for (int i = 0; i < 6; i++)
	{
		terrainFaces[i] = new TerrainFace(shapeGenerator, meshes[i], resolution, directions[i]);
	}
}

void APlanet::GenerateMesh()
{
	for (auto& face : terrainFaces)
	{
		face->ConstructMesh();
	}
}

void APlanet::OnShapeSettingsUpdated()
{
	Initialize();
	GenerateMesh();
}

void APlanet::OnColorSettingsUpdated()
{
	Initialize();
	GenerateColors();
}


void APlanet::GenerateColors()
{
	if (ColorSettings != nullptr)
	{
		for (int i = 0; i < 6; i++)
		{
			terrainFaces[i]->ColorMesh(ColorSettings);
		}
	}
}

void APlanet::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, resolution))
		{
			GeneratePlanet();
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, ShapeSettings) && PropertyChangedEvent.Property->IsValidLowLevel())
		{
			GeneratePlanet();
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, ColorSettings) && PropertyChangedEvent.Property->IsValidLowLevel())
		{
			GeneratePlanet();
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
