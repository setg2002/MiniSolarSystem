// Copyright Soren Gilbertson


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
	
	ColorGenerator = new GaseousColorGenerator();
}


void URingSystemComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	UpdateProperties();
}

void URingSystemComponent::CreateMaterial()
{
	DynamicMaterial = this->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/Instances/M_Ring_Inst.M_Ring_Inst'"), NULL, LOAD_None, NULL));
	DynamicMaterial->SetScalarParameterValue("_ringWidth", RingWidth);
	if (Gradient)
	{
		GradientTexture = ColorGenerator->CreateTexture("RingTexture", Gradient);
		DynamicMaterial->SetTextureParameterValue(FName("_Gradient"), GradientTexture);
	}
}

void URingSystemComponent::SetRadius(float NewRadius)
{
	Radius = NewRadius;
	this->SetRelativeScale3D(GetOwner()->GetActorScale() * Radius * 6);
}

void URingSystemComponent::SetWidth(float NewWidth)
{
	RingWidth = NewWidth;
	DynamicMaterial->SetScalarParameterValue("_ringWidth", RingWidth);
}

// Called when the game starts
void URingSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateMaterial();
}

void URingSystemComponent::UpdateProperties()
{
	this->SetRelativeScale3D(GetOwner()->GetActorScale() * Radius * 6);
	this->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"), NULL, LOAD_None, NULL));
	this->bCastVolumetricTranslucentShadow = false; // Set to true for the ring to cast shadows on the planet with the side-effect that the shading of the ring itself will break
	CreateMaterial();
	DynamicMaterial->SetScalarParameterValue("_ringWidth", RingWidth);
	if (Gradient)
	{
		GradientTexture = ColorGenerator->CreateTexture("RingTexture", Gradient);
		DynamicMaterial->SetTextureParameterValue(FName("_Gradient"), GradientTexture);
	}
}


// Called every frame
void URingSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

#if WITH_EDITOR
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
			if (!DynamicMaterial)
			{
				CreateMaterial();
			}
			DynamicMaterial->SetScalarParameterValue("_ringWidth", RingWidth);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(URingSystemComponent, Gradient) && Gradient != nullptr)
		{
			if (!DynamicMaterial)
			{
				CreateMaterial();
			}
			GradientTexture = ColorGenerator->CreateTexture("RingTexture", Gradient);
			DynamicMaterial->SetTextureParameterValue(FName("_Gradient"), GradientTexture);
		}
	}
}
#endif
