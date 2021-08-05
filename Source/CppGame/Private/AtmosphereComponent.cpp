// Copyright Soren Gilbertson


#include "AtmosphereComponent.h"
#include "Planet.h"
#include "ShapeSettings.h"
#include "CloudComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


UAtmosphereComponent::UAtmosphereComponent()
{
	this->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/StaticMeshes/Atmosphere_mesh.Atmosphere_mesh'"), NULL, LOAD_None, NULL));

	CloudComponent = CreateDefaultSubobject<UCloudComponent>("Clouds");
	CloudComponent->SetRelativeLocation(FVector::ZeroVector);
#if WITH_EDITOR
	CloudComponent->CreationMethod = EComponentCreationMethod::Instance;
#endif
}


void UAtmosphereComponent::SetClouds(bool NewClouds)
{
	bClouds = NewClouds;
	CloudComponent->SetVisibility(bClouds);
}

void UAtmosphereComponent::SetCloudHeight(float NewCloudHeight)
{
	CloudHeight = NewCloudHeight;
	CloudComponent->SetRelativeScale3D(FVector(CloudHeight));
}


void UAtmosphereComponent::SetHeight(float NewHeight)
{
	AtmosphereProperties.Height = NewHeight;
	DynamicMaterial->SetScalarParameterValue("atmo_radius", AtmosphereProperties.Height);
}

void UAtmosphereComponent::SetDensity(float NewDensity)
{
	AtmosphereProperties.Density = NewDensity;
	DynamicMaterial->SetScalarParameterValue("density_multiplier", AtmosphereProperties.Density);
}

void UAtmosphereComponent::SetG(float NewG)
{
	AtmosphereProperties.g = NewG;
	DynamicMaterial->SetScalarParameterValue("g", AtmosphereProperties.g);
}

void UAtmosphereComponent::SetMieHeightScale(float NewMieHeightScale)
{
	AtmosphereProperties.MieHeightScale = NewMieHeightScale;
	DynamicMaterial->SetScalarParameterValue("mie_height_scale", AtmosphereProperties.MieHeightScale);
}

void UAtmosphereComponent::SetOpacityMultiplier(float NewOpacityMultiplier)
{
	AtmosphereProperties.OpacityMultiplier = NewOpacityMultiplier;
	DynamicMaterial->SetScalarParameterValue("opacity_multiplier", AtmosphereProperties.OpacityMultiplier);
}

void UAtmosphereComponent::SetAmbient(FLinearColor NewAmbient)
{
	AtmosphereProperties.Ambient = NewAmbient;
	DynamicMaterial->SetVectorParameterValue("beta_ambient", AtmosphereProperties.Ambient);
}

void UAtmosphereComponent::SetMie(FLinearColor NewMie)
{
	AtmosphereProperties.Mie = NewMie;
	DynamicMaterial->SetVectorParameterValue("beta_mie", AtmosphereProperties.Mie);
}

void UAtmosphereComponent::SetRay(FLinearColor NewRay)
{
	AtmosphereProperties.Ray = NewRay;
	DynamicMaterial->SetVectorParameterValue("beta_ray", AtmosphereProperties.Ray);
}


void UAtmosphereComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

}


void UAtmosphereComponent::DestroyComponent(bool bPromoteChildren)
{
	if (CloudComponent != nullptr)
	{
		CloudComponent->DestroyComponent();
	}

	Super::DestroyComponent(bPromoteChildren);
}

void UAtmosphereComponent::BeginPlay()
{
	Super::BeginPlay();

	Cast<ACelestialBody>(GetOwner())->AddCelestialComponent(CloudComponent);

	DynamicMaterial = this->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Materials/Instances/M_atmosphere_proportional_Inst.M_atmosphere_proportional_Inst'"), NULL, LOAD_None, NULL));
	UpdateProperties();
	CloudComponent->CreateMaterial();
}

void UAtmosphereComponent::UpdateProperties()
{
	PlanetRadius = Cast<APlanet>(GetOwner())->ShapeSettings->GetRadius();
	this->SetRelativeLocation(FVector::ZeroVector);
	this->SetRelativeScale3D(FVector(PlanetRadius * 0.0125f));

	SetClouds(bClouds);
	SetCloudHeight(CloudHeight);

	DynamicMaterial->SetScalarParameterValue("planet_radius", PlanetRadius);
	DynamicMaterial->SetScalarParameterValue("atmo_radius", AtmosphereProperties.Height);
	DynamicMaterial->SetScalarParameterValue("density_multiplier", AtmosphereProperties.Density);
	DynamicMaterial->SetScalarParameterValue("g", AtmosphereProperties.g);
	DynamicMaterial->SetScalarParameterValue("mie_height_scale", AtmosphereProperties.MieHeightScale);
	DynamicMaterial->SetScalarParameterValue("opacity_multiplier", AtmosphereProperties.OpacityMultiplier);
	DynamicMaterial->SetVectorParameterValue("beta_ambient", AtmosphereProperties.Ambient);
	DynamicMaterial->SetVectorParameterValue("beta_mie", AtmosphereProperties.Mie);
	DynamicMaterial->SetVectorParameterValue("beta_ray", AtmosphereProperties.Ray);
}

#if WITH_EDITOR
void UAtmosphereComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(FAtmosphereProperties, Height))
		{
			DynamicMaterial->SetScalarParameterValue("atmo_radius", AtmosphereProperties.Height);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FAtmosphereProperties, Density))
		{
			DynamicMaterial->SetScalarParameterValue("density_multiplier", AtmosphereProperties.Density);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FAtmosphereProperties, g))
		{
			DynamicMaterial->SetScalarParameterValue("g", AtmosphereProperties.g);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FAtmosphereProperties, MieHeightScale))
		{
			DynamicMaterial->SetScalarParameterValue("mie_height_scale", AtmosphereProperties.MieHeightScale);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FAtmosphereProperties, OpacityMultiplier))
		{
			DynamicMaterial->SetScalarParameterValue("opacity_multiplier", AtmosphereProperties.OpacityMultiplier);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FAtmosphereProperties, Steps_i))
		{
			DynamicMaterial->SetScalarParameterValue("steps_i", AtmosphereProperties.Steps_i);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FAtmosphereProperties, Steps_j))
		{
			DynamicMaterial->SetScalarParameterValue("steps_j", AtmosphereProperties.Steps_j);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FAtmosphereProperties, Ambient))
		{
			DynamicMaterial->SetVectorParameterValue("beta_ambient", AtmosphereProperties.Ambient);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FAtmosphereProperties, Mie))
		{
			DynamicMaterial->SetVectorParameterValue("beta_mie", AtmosphereProperties.Mie);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FAtmosphereProperties, Ray))
		{
			DynamicMaterial->SetVectorParameterValue("beta_ray", AtmosphereProperties.Ray);
		}

		if (PropertyName == GET_MEMBER_NAME_CHECKED(UAtmosphereComponent, bClouds))
		{
			CloudComponent->SetVisibility(bClouds);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(UAtmosphereComponent, CloudHeight))
		{
			CloudComponent->SetRelativeScale3D(FVector(CloudHeight)); //HACK This should not be here, only the cloud component itself should control this 
		}
	}
}
#endif
