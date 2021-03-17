// This is a copyright notice


#include "Star.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"


AStar::AStar()
{
	sphere = CreateDefaultSubobject<UStaticMeshComponent>(FName("Sphere"));
	sphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	this->initialVelocity = FVector::ZeroVector;

	dynamicMaterial = sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, sphere->GetMaterial(0));
	sphere->SetMaterial(0, dynamicMaterial);
}

void AStar::UpdateColor()
{
	if (sphere->GetMaterial(0) != dynamicMaterial)
	{
		dynamicMaterial = sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, sphere->GetMaterial(0));
		sphere->SetMaterial(0, dynamicMaterial);
	}
	dynamicMaterial->SetVectorParameterValue(FName("_baseColor"), starProperties.color);
}

/*void AStar::GenerateDistantStars()
{
	if (distantStars.Num() < numDistantStars)
	{
		for (int i = distantStars.Num(); i < numDistantStars; i++)
		{
			FVector spawnLocation = FVector::ZeroVector;
			float dist = FVector::Dist(spawnLocation, FVector(0, 0, 0));
			while (dist > maxSpawnRange || dist < minSpawnRange)
			{
				spawnLocation.X = FMath::FRandRange(-maxSpawnRange, maxSpawnRange);
				spawnLocation.Y = FMath::FRandRange(-maxSpawnRange, maxSpawnRange);
				spawnLocation.Z = FMath::FRandRange(-maxSpawnRange, maxSpawnRange);
				dist = FVector::Dist(spawnLocation, FVector(0, 0, 0));
			}

			FActorSpawnParameters spawnInfo;
			distantStars.Add(GetWorld()->SpawnActor<AActor>(distantStarBP, spawnLocation, FRotator(0, 0, 0), spawnInfo));
			int randColor = FMath::FRandRange(0, possibleColors.Num());
			ensure(Cast<UMeshComponent>(distantStars.Last()->GetRootComponent()));
			UMeshComponent* comp = Cast<UMeshComponent>(distantStars.Last()->GetRootComponent());
			comp->SetMaterial(0, dynamMats[randColor]);

			float newRadius = FMath::FRandRange(1, 20);
			distantStars.Last()->SetActorScale3D(FVector(newRadius, newRadius, newRadius));
		}
	}
	else
	{
		while (distantStars.Num() > numDistantStars)
		{
			distantStars[distantStars.Num() - 1]->Destroy();
			distantStars.RemoveAt(distantStars.Num() - 1, 1, true);
		}
	}
}*/

void AStar::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		/*if (PropertyName == GET_MEMBER_NAME_CHECKED(AStar, possibleColors))
		{
			for (int i = 0; i < possibleColors.Num(); i++)
			{
				dynamMats.EmplaceAt(i, UMaterialInstanceDynamic::Create(distStarMat, this));
				dynamMats[i]->SetVectorParameterValue(FName("_baseColor"), possibleColors[i]);
			}
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(AStar, numDistantStars))
		{
			GenerateDistantStars();
		}*/
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FStarProperties, radius))
		{
			sphere->SetRelativeScale3D(FVector(starProperties.radius, starProperties.radius, starProperties.radius));
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FStarProperties, mass))
		{
			this->mass = starProperties.mass;
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FStarProperties, color))
		{
			if (sphere->GetMaterial(0) != dynamicMaterial)
			{
				dynamicMaterial = sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, sphere->GetMaterial(0));
				sphere->SetMaterial(0, dynamicMaterial);
			}
			if (planetMateralParameterCollectionInst == nullptr)
			{
				planetMateralParameterCollectionInst = GetWorld()->GetParameterCollectionInstance(planetMateralParameterCollection);
			}
			UpdateColor();
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FStarProperties, luminosity))
		{
			if (sphere->GetMaterial(0) != dynamicMaterial)
			{
				dynamicMaterial = sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, sphere->GetMaterial(0));
				sphere->SetMaterial(0, dynamicMaterial);
			}
			if (planetMateralParameterCollectionInst == nullptr)
			{
				planetMateralParameterCollectionInst = GetWorld()->GetParameterCollectionInstance(planetMateralParameterCollection);
			}
			dynamicMaterial->SetScalarParameterValue(FName("_glowPower"), starProperties.luminosity);
			planetMateralParameterCollectionInst->SetScalarParameterValue(FName("StarLuminosity"), starProperties.luminosity);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(AStar, starType))
		{
			static const FString ContextString(TEXT("Star Type"));
			starProperties = *(starTypeData->FindRow<FStarProperties>(FName(UEnum::GetValueAsString<EStarType>(starType.GetValue())), ContextString, true));
			sphere->SetRelativeScale3D(FVector(starProperties.radius, starProperties.radius, starProperties.radius));
			this->mass = starProperties.mass;

			if (sphere->GetMaterial(0) != dynamicMaterial)
			{
				dynamicMaterial = sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, sphere->GetMaterial(0));
				sphere->SetMaterial(0, dynamicMaterial);
			}
			dynamicMaterial->SetVectorParameterValue(FName("_baseColor"), starProperties.color);
			dynamicMaterial->SetScalarParameterValue(FName("_glowPower"), starProperties.luminosity);
		}
		/*if (PropertyName == GET_MEMBER_NAME_CHECKED(USceneComponent, RelativeLocation) || PropertyName == GET_MEMBER_NAME_CHECKED(FVector, X) || PropertyName == GET_MEMBER_NAME_CHECKED(FVector, Y) || PropertyName == GET_MEMBER_NAME_CHECKED(FVector, Z))
		{
			if (planetMateralParameterCollectionInst == nullptr)
			{	
				planetMateralParameterCollectionInst = GetWorld()->GetParameterCollectionInstance(planetMateralParameterCollection);
			}
			planetMateralParameterCollectionInst->SetVectorParameterValue(FName("SunLocation"), this->GetActorLocation());
		}*/
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
