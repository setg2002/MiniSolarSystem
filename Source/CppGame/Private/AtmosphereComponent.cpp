// This is a copyright notice


#include "AtmosphereComponent.h"
#include "Planet.h"
#include "ShapeSettings.h"
#include "CloudComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


UAtmosphereComponent::UAtmosphereComponent(/*float radius*/)
{

	CloudComponent = CreateDefaultSubobject<UCloudComponent>("Clouds");
}


void UAtmosphereComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	PlanetRadius = Cast<APlanet>(GetOwner())->ShapeSettings->PlanetRadius; //TODO Use constructor passed radius in full game

	this->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	this->SetRelativeScale3D(FVector(PlanetRadius * 0.0125f));
	this->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/StaticMeshes/Atmosphere_mesh.Atmosphere_mesh'"), NULL, LOAD_None, NULL));
	DynamicMaterial = nullptr;
	DynamicMaterial = this->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/Instances/M_atmosphere_proportional_Inst.M_atmosphere_proportional_Inst'"), NULL, LOAD_None, NULL));
	DynamicMaterial->SetScalarParameterValue("planet_radius", PlanetRadius);
}


void UAtmosphereComponent::DestroyComponent(bool bPromoteChildren)
{
	if (CloudComponent != nullptr)
	{
		CloudComponent->DestroyComponent();
	}

	Super::DestroyComponent(bPromoteChildren);
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
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FAtmosphereProperties, Intensity))
		{
			DynamicMaterial->SetScalarParameterValue("Intensity", AtmosphereProperties.Intensity);
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
