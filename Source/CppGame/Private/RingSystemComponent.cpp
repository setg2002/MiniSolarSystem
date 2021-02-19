// This is a copyright notice


#include "RingSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/UObjectGlobals.h"


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

	RingMesh->DestroyComponent();
}


void URingSystemComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	
	RingMesh = NewObject<UStaticMeshComponent>(GetOwner(), UStaticMeshComponent::StaticClass(), "RingMesh", RF_NoFlags, nullptr, false, nullptr, GetOwner()->GetPackage());
	RingMesh->AttachTo(GetOwner()->GetRootComponent());
	RingMesh->RegisterComponent();
	RingMesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"), NULL, LOAD_None, NULL));
	RingMesh->SetMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/RingMat_Inst.RingMat_Inst'"), NULL, LOAD_None, NULL)); //TODO Fix bad hard-coded ref 
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

