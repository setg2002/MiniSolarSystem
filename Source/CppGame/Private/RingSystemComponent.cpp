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


void URingSystemComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	
	RingMesh = NewObject<UStaticMeshComponent>(GetOwner(), UStaticMeshComponent::StaticClass(), "RingMesh", RF_NoFlags, nullptr, false, nullptr, GetOwner()->GetPackage());
	RingMesh->AttachTo(GetOwner()->GetRootComponent());
	RingMesh->RegisterComponent();
	RingMesh->SetRelativeScale3D(GetOwner()->GetActorScale() * OuterRadius * 6);
	RingMesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"), NULL, LOAD_None, NULL));
	DynamicMaterial = RingMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/RingMat_Inst.RingMat_Inst'"), NULL, LOAD_None, NULL)); //TODO Fix bad hard-coded ref 
	DynamicMaterial->SetScalarParameterValue("_innerRadius", InnerRadius);
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

		if (PropertyName == GET_MEMBER_NAME_CHECKED(URingSystemComponent, OuterRadius))
		{
			RingMesh->SetRelativeScale3D(GetOwner()->GetActorScale() * OuterRadius * 6);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(URingSystemComponent, InnerRadius))
		{
			DynamicMaterial->SetScalarParameterValue("_innerRadius", InnerRadius);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(URingSystemComponent, Gradient) && Gradient != nullptr)
		{
			//TODO This is brokem because of some threading issue, no clue, possibly an engine bug: fix later
			//GradientTexture = Cast<AGasGiant>(GetOwner())->CreateTexture("RingTexture", Gradient);
			//DynamicMaterial->SetTextureParameterValueByInfo(FMaterialParameterInfo("_Gradient"), GradientTexture);
		}
	}
}
