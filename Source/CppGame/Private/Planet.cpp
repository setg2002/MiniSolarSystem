// This is a copyright notice


#include "Planet.h"

APlanet::APlanet() 
{
	StaticMeshes = { CreateDefaultSubobject<UStaticMeshComponent>("Mesh"),
					 CreateDefaultSubobject<UStaticMeshComponent>("Mesh1"),
					 CreateDefaultSubobject<UStaticMeshComponent>("Mesh2"),
					 CreateDefaultSubobject<UStaticMeshComponent>("Mesh3"),
					 CreateDefaultSubobject<UStaticMeshComponent>("Mesh4"),
					 CreateDefaultSubobject<UStaticMeshComponent>("Mesh5")
	};

	ProcMeshes = { CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh"),
			   CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh1"),
			   CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh2"),
			   CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh3"),
			   CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh4"),
			   CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh5")
	};

	shapeGenerator = new ShapeGenerator();
	colorGenerator = new ColorGenerator();

	GeneratePlanet();

}

void APlanet::GeneratePlanet()
{
	if (ColorSettings)
	{
		for (auto& Mesh : ProcMeshes)
		{
			Mesh->SetRelativeLocation(FVector().ZeroVector);
		}
		for (auto& Mesh : StaticMeshes)
		{
			Mesh->SetRelativeLocation(FVector().ZeroVector);
		}

		if (ShapeSettings != nullptr && ShapeSettings->GetNoiseLayers())
		{
			Initialize();
			GenerateMesh();
			GenerateColors();
		}
	}
}

void APlanet::ReGenerate()
{
	GeneratePlanet();
}

void APlanet::ReGenerateColors()
{
	OnColorSettingsUpdated();
}

void APlanet::ReGenerateTangents()
{
	if (bMultithreadGeneration)
	{
		for (int i = 0; i < 6; i++)
		{
			terrainFaces[i]->UpdateTangentsNormalsAsync();
		}
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			terrainFaces[i]->UpdateTangentsNormals();
		}
	}
}

void APlanet::CreateMesh()
{
	/*for (int i = 0; i < 6; i++)
	{
		terrainFaces[i]->CreateMesh();
	}*/
}

void APlanet::Initialize()
{
	shapeGenerator->UpdateSettings(ShapeSettings);
	colorGenerator->UpdateSettings(ColorSettings);
		
	for (int32 i = 0; i < 6; i++)
	{
		terrainFaces[i] = new TerrainFace(shapeGenerator, colorGenerator, resolution, directions[i], ProcMeshes[i], StaticMeshes[i], i);

		if (ColorSettings->DynamicMaterials[i] == nullptr)
		{
			ColorSettings->DynamicMaterials[i] = ProcMeshes[i]->CreateAndSetMaterialInstanceDynamicFromMaterial(0, ColorSettings->PlanetMat);
		}

		bool renderFace = FaceRenderMask == EFaceRenderMask::All || FaceRenderMask - 1 == i;
		ProcMeshes[i]->SetVisibility(renderFace);
	}
}

void APlanet::GenerateMesh()
{
	if (bMultithreadGeneration)
	{
		for (int i = 0; i < 6; i++)
		{
			terrainFaces[i]->ConstructMeshAsync(colorGenerator);
		}
	}
	else 
	{
		for (int i = 0; i < 6; i++)
		{
			if (ProcMeshes[i]->IsVisibleInEditor())
			{
				terrainFaces[i]->ConstructMesh(colorGenerator);
			}
		}
		colorGenerator->UpdateElevation(shapeGenerator->ElevationMinMax);
	}
}

void APlanet::OnShapeSettingsUpdated()
{
	Initialize();
	GenerateMesh();
}

void APlanet::OnColorSettingsUpdated()
{
	GenerateColors();
}

void APlanet::GenerateColors()
{
	colorGenerator->UpdateColors();
	if (bMultithreadGeneration)
	{
		colorGenerator->UpdateElevation(shapeGenerator->ElevationMinMax);
	}
}

void APlanet::CalculateOrbitVelocity()
{
	if (OrbitingBody == this || OrbitingBody == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OrbitingBody cannot be null or itself"));
		return;
	}
	else
	{
		//TODO Fix orbit velocity calculation
		//UE_LOG(LogTemp, Warning, TEXT("Distance: %d"), (OrbitingBody->GetActorLocation() - this->GetActorLocation()).Size());
		//orbitVelocity = FMath::Sqrt(((/*gameMode->gravitationalConstant*/100 * 100000) * OrbitingBody->mass) / 11110);
		return;
	}
}

void APlanet::SetToOrbit()
{
	//TODO Calculate what portion of the necessary orbit velocity should be applied to each sectopn of the initialVelocity vector
	initialVelocity.Y = orbitVelocity;
}

void APlanet::ConvertAndSetStaticMeshes(int32 i)
{
	FString ActorName = this->GetName();

	StaticMeshes[i]->SetStaticMesh(terrainFaces[i]->ConvertToStaticMesh(ActorName));
	ProcMeshes[i]->ClearMeshSection(0);
}



void APlanet::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, resolution) && bAutoGenerate)
		{
			GeneratePlanet();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, ShapeSettings) && bAutoGenerate)
		{
			GeneratePlanet();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, ColorSettings) && bAutoGenerate)
		{
			GeneratePlanet();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APlanet, FaceRenderMask) && bAutoGenerate)
		{
			GeneratePlanet();
			if (bAutoGenerateTangents) { ReGenerateTangents(); }
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
