// This is a copyright notice


#include "Star.h"
#include "G:\UESource\Engine\Source\Runtime\Engine\Classes\Materials\MaterialInstanceDynamic.h"
#include "G:\UESource\Engine\Source\Runtime\Engine\Classes\Materials\MaterialParameterCollection.h"
#include "G:\UESource\Engine\Source\Runtime\Engine\Classes\Materials\MaterialParameterCollectionInstance.h"


AStar::AStar()
{
	sphere = CreateDefaultSubobject<UStaticMeshComponent>(FName("Sphere"));
	sphere->AttachTo(RootComponent);

	dynamicMaterial = sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, sphere->GetMaterial(0));
	sphere->SetMaterial(0, dynamicMaterial);
}

void AStar::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(FStarProperties, radius))
		{
			sphere->SetRelativeScale3D(FVector(starProperties.radius, starProperties.radius, starProperties.radius));
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FStarProperties, mass))
		{
			this->mass = starProperties.mass;
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FStarProperties, color))
		{
			if (sphere->GetMaterial(0) != dynamicMaterial)
			{
				dynamicMaterial = sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, sphere->GetMaterial(0));
				sphere->SetMaterial(0, dynamicMaterial);
			}
			if (planetMateralParameterCollectionInst == nullptr)
			{
				planetMateralParameterCollectionInst = GetWorld()->GetParameterCollectionInstance(planetMateralParameterCollection);
			}
			dynamicMaterial->SetVectorParameterValue(FName("_baseColor"), starProperties.color);
			planetMateralParameterCollectionInst->SetVectorParameterValue(FName("SunColor"), starProperties.color);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FStarProperties, luminosity))
		{
			if (sphere->GetMaterial(0) != dynamicMaterial)
			{
				dynamicMaterial = sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, sphere->GetMaterial(0));
				sphere->SetMaterial(0, dynamicMaterial);
			}
			if (planetMateralParameterCollectionInst == nullptr)
			{
				planetMateralParameterCollectionInst = GetWorld()->GetParameterCollectionInstance(planetMateralParameterCollection);
			}
			dynamicMaterial->SetScalarParameterValue(FName("_glowPower"), starProperties.luminosity);
			planetMateralParameterCollectionInst->SetScalarParameterValue(FName("StarLuminocity"), starProperties.luminosity);
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(AStar, starType))
		{
			static const FString ContextString(TEXT("Star Type"));
			starProperties = *(starTypeData->FindRow<FStarProperties>(FName(UEnum::GetValueAsString<EStarType>(starType.GetValue())), ContextString, true));
			sphere->SetRelativeScale3D(FVector(starProperties.radius, starProperties.radius, starProperties.radius));
			this->mass = starProperties.mass;

			if (sphere->GetMaterial(0) != dynamicMaterial)
			{
				dynamicMaterial = sphere->CreateAndSetMaterialInstanceDynamicFromMaterial(0, sphere->GetMaterial(0));
				sphere->SetMaterial(0, dynamicMaterial);
			}
			dynamicMaterial->SetVectorParameterValue(FName("_baseColor"), starProperties.color);
			dynamicMaterial->SetScalarParameterValue(FName("_glowPower"), starProperties.luminosity);
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
