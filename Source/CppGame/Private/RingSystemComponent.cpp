// This is a copyright notice


#include "RingSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GasGiant.h"



// Sets default values for this component's properties
URingSystemComponent::URingSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void URingSystemComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	if (RingMesh)
	{
		RingMesh->DestroyComponent();
	}
}


void URingSystemComponent::CreateMaterial()
{
	DynamicMaterial = RingMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/RingMat_Inst.RingMat_Inst'"), NULL, LOAD_None, NULL)); //TODO Fix bad hard-coded ref?
}


void URingSystemComponent::FindRingMesh()
{
	TArray<UActorComponent*> Components = GetOwner()->GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Ring"));
	if (Components.IsValidIndex(0))
	{
		RingMesh = Cast<UStaticMeshComponent>(Components[0]);
		RingMesh->RegisterComponent();
		RingMesh->SetRelativeScale3D(GetOwner()->GetActorScale() * Radius * 6);
		CreateMaterial();
		DynamicMaterial->SetScalarParameterValue("_ringWidth", RingWidth);
		if (Gradient)
		{
			GradientTexture = Cast<AGasGiant>(GetOwner())->CreateTexture("RingTexture", Gradient);
			DynamicMaterial->SetTextureParameterValue(FName("_Gradient"), GradientTexture);
		}
	}
	else
	{
		RingMesh = NewObject<UStaticMeshComponent>(GetOwner(), UStaticMeshComponent::StaticClass(), "RingMesh", RF_NoFlags, nullptr, false, nullptr, GetOwner()->GetPackage());
		RingMesh->ComponentTags.Add(FName("Ring"));
		GetOwner()->AddOwnedComponent(RingMesh);
		RingMesh->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		RingMesh->RegisterComponent();
		RingMesh->SetRelativeScale3D(GetOwner()->GetActorScale() * Radius * 6);
		RingMesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"), NULL, LOAD_None, NULL));
		CreateMaterial();
		DynamicMaterial->SetScalarParameterValue("_ringWidth", RingWidth);
		if (Gradient)
		{
			GradientTexture = Cast<AGasGiant>(GetOwner())->CreateTexture("RingTexture", Gradient);
			DynamicMaterial->SetTextureParameterValue(FName("_Gradient"), GradientTexture);
		}
	}
}


void URingSystemComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	FindRingMesh();
}


// Called when the game starts
void URingSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URingSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void URingSystemComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(URingSystemComponent, Radius))
		{
			if (!RingMesh)
			{
				FindRingMesh();
			}
			RingMesh->SetRelativeScale3D(GetOwner()->GetActorScale() * Radius * 6);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(URingSystemComponent, RingWidth))
		{
			if (!RingMesh)
			{
				FindRingMesh();
			}
			DynamicMaterial->SetScalarParameterValue("_ringWidth", RingWidth);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(URingSystemComponent, Gradient) && Gradient != nullptr)
		{
			if (!RingMesh)
			{
				FindRingMesh();
			}
			CreateMaterial();
			GradientTexture = Cast<AGasGiant>(GetOwner())->CreateTexture("RingTexture", Gradient);
			DynamicMaterial->SetTextureParameterValue(FName("_Gradient"), GradientTexture);
		}
	}
}
