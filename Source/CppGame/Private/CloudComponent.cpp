// Copyright Soren Gilbertson


#include "CloudComponent.h"
#include "AtmosphereComponent.h"

UCloudComponent::UCloudComponent()
{
}

void UCloudComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

}

void UCloudComponent::BeginPlay()
{
	Super::BeginPlay();

	this->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	this->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/StaticMeshes/UVSphere.UVSphere'"), NULL, LOAD_None, NULL));
	this->bCastVolumetricTranslucentShadow = false;
	DynamicMaterial = this->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/Instances/M_Clouds_Inst.M_Clouds_Inst'"), NULL, LOAD_None, NULL));
}

#if WITH_EDITOR
void UCloudComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(UCloudComponent, CloudHeight))
		{
			this->SetRelativeScale3D(FVector(CloudHeight));
		}
	}
}
#endif
