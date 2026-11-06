// Copyright Soren Gilbertson


#include "Bodies/Components/CloudComponent.h"
#include "Bodies/Components/AtmosphereComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"

UCloudComponent::UCloudComponent()
{
	UStaticMeshComponent::SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/StaticMeshes/UVSphere.UVSphere'"), NULL, LOAD_None, NULL));
	bCastVolumetricTranslucentShadow = false;
}

void UCloudComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

}

void UCloudComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCloudComponent::CreateMaterial()
{
	DynamicMaterial = CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Materials/Instances/M_Clouds_Inst.M_Clouds_Inst'"), NULL, LOAD_None, NULL));
}

#if WITH_EDITOR
void UCloudComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		/*if (PropertyName == GET_MEMBER_NAME_CHECKED(UCloudComponent, CloudHeight))
		{
			this->SetRelativeScale3D(FVector(CloudHeight));
		}*/
	}
}
#endif
