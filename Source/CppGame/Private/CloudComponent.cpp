// Copyright Soren Gilbertson


#include "CloudComponent.h"
#include "AtmosphereComponent.h"

UCloudComponent::UCloudComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;


	this->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/StaticMeshes/UVSphere.UVSphere'"), NULL, LOAD_None, NULL));
	this->bCastVolumetricTranslucentShadow = false;
}

void UCloudComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

}

void UCloudComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCloudComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (this->GetRelativeLocation() != FVector::ZeroVector)
	{
		this->SetRelativeLocation(FVector::ZeroVector);
	}
}

void UCloudComponent::CreateMaterial()
{
	DynamicMaterial = this->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/Instances/M_Clouds_Inst.M_Clouds_Inst'"), NULL, LOAD_None, NULL));
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
