// Copyright Soren Gilbertson


#include "Skybox.h"
#include "AssetRegistryModule.h"
#include "Materials/MaterialInstanceDynamic.h"


// Sets default values
ASkybox::ASkybox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"), NULL, LOAD_None, NULL));
	Mesh->SetWorldScale3D(FVector(1000000));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->CastShadow = 0;
	Mesh->bCastDynamicShadow = 0;
	RootComponent = Mesh;
}


void ASkybox::OnConstruction(const FTransform & Transform)
{
	DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/Instances/M_SkyBox_Inst.M_SkyBox_Inst'"), NULL, LOAD_None, NULL));
	//FString PackageName = TEXT("UTexture2D'/Game/ProceduralTextures/" + this->GetName() + "_Skybox'");
	//DynamicMaterial->SetTextureParameterValue("StarTexture", LoadObject<UTexture2D>(NULL, *PackageName, NULL, LOAD_None, NULL));
}


// Called when the game starts or when spawned
void ASkybox::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ASkybox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// This is very slow
UTexture2D* ASkybox::MakeTexture()
{
	int16 TextureRes = 2048;
	int32 NumStars = 50000;

	TArray<FVector2D> Points;
	for (int32 i = 0; i < NumStars; ++i)
	{
		Points.AddUnique(FVector2D(FMath::RandRange(0, TextureRes), FMath::RandRange(0, TextureRes)));
	}

	UTexture2D* DynamicTexture = UTexture2D::CreateTransient(TextureRes, TextureRes, EPixelFormat::PF_B8G8R8A8);

	DynamicTexture->UpdateResource();

	uint8* Pixels = new uint8[TextureRes * TextureRes * 4];
	for (int32 y = 0; y < TextureRes; y++)
	{
		for (int32 x = 0; x < TextureRes; x++)
		{
			FColor color;
			
			if (Points.Contains(FVector2D(x, y)))
			{
				color = possibleColors[FMath::RandRange(0, 5)];
			}
			else
			{
				color = FColor::Black;
			}

			int32 curPixelIndex = ((y * TextureRes) + x);
			Pixels[4 * curPixelIndex] = color.B;
			Pixels[4 * curPixelIndex + 1] = color.G;
			Pixels[4 * curPixelIndex + 2] = color.R;
			Pixels[4 * curPixelIndex + 3] = 255;
		}
	}

	FUpdateTextureRegion2D* Region = new FUpdateTextureRegion2D;
	Region->DestX = 0;
	Region->DestY = 0;
	Region->SrcX = 0;
	Region->SrcY = 0;
	Region->Width = TextureRes;
	Region->Height = TextureRes;

	TFunction<void(uint8* SrcData, const FUpdateTextureRegion2D* Regions)> DataCleanupFunc =
		[](uint8* SrcData, const FUpdateTextureRegion2D* Regions) {
		delete[] SrcData;
		delete[] Regions;
	};

	DynamicTexture->UpdateTextureRegions(0, 1, Region, TextureRes * 4, 4, Pixels);

	return DynamicTexture;
}


#if WITH_EDITOR
void ASkybox::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(FVector2D, X) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(FVector2D, Y) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(ASkybox, NoiseWarpAmount) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(ASkybox, ColorMultiplier) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(ASkybox, DesaturationAmount) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(ASkybox, Intensity))
		{
			DynamicMaterial->SetVectorParameterValue("StarOffset", FLinearColor(StarOffset.X, StarOffset.Y, 0, 0));
			DynamicMaterial->SetVectorParameterValue("NoiseOffset", FLinearColor(NoiseOffset.X, NoiseOffset.Y, 0, 0));
			DynamicMaterial->SetVectorParameterValue("ColorOffset", FLinearColor(ColorOffset.X, ColorOffset.Y, 0, 0));
			DynamicMaterial->SetScalarParameterValue("NoiseWarpAmount", NoiseWarpAmount);
			DynamicMaterial->SetScalarParameterValue("ColorMultiplier", ColorMultiplier);
			DynamicMaterial->SetScalarParameterValue("DesaturationAmount", DesaturationAmount);
			DynamicMaterial->SetScalarParameterValue("Intensity", Intensity);
		}
	}
}
#endif
