// This is a copyright notice


#include "RingSystemComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GaseousColorGenerator.h"



// Sets default values for this component's properties
URingSystemComponent::URingSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	ColorGenerator = new GaseousColorGenerator(this);
}


void URingSystemComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	this->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	this->SetRelativeScale3D(GetOwner()->GetActorScale() * Radius * 6);
	this->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"), NULL, LOAD_None, NULL));
	DynamicMaterial = this->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/RingMat_Inst.RingMat_Inst'"), NULL, LOAD_None, NULL)); //TODO Fix bad hard-coded ref?
	DynamicMaterial->SetScalarParameterValue("_ringWidth", RingWidth);
	if (Gradient)
	{
		GradientTexture = ColorGenerator->CreateTexture("RingTexture", Gradient);
		DynamicMaterial->SetTextureParameterValue(FName("_Gradient"), GradientTexture);
	}
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
			this->SetRelativeScale3D(GetOwner()->GetActorScale() * Radius * 6);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(URingSystemComponent, RingWidth))
		{
			DynamicMaterial->SetScalarParameterValue("_ringWidth", RingWidth);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(URingSystemComponent, Gradient) && Gradient != nullptr)
		{
			DynamicMaterial = this->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/RingMat_Inst.RingMat_Inst'"), NULL, LOAD_None, NULL)); //TODO Fix bad hard-coded ref?
			GradientTexture = ColorGenerator->CreateTexture("RingTexture", Gradient);
			DynamicMaterial->SetTextureParameterValue(FName("_Gradient"), GradientTexture);
		}
	}
}
