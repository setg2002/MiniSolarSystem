// Copyright Soren Gilbertson


#include "Skybox.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/SavePackage.h"


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
	if (DynamicMaterial == nullptr)
	{
		DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Materials/Instances/M_SkyBox_Inst.M_SkyBox_Inst'"), NULL, LOAD_None, NULL));
		//FString PackageName = TEXT("UTexture2D'/Game/ProceduralTextures/" + this->GetName() + "_Skybox'");
		//DynamicMaterial->SetTextureParameterValue("StarTexture", LoadObject<UTexture2D>(NULL, *PackageName, NULL, LOAD_None, NULL));
	}	
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
#if WITH_EDITOR
UE_DISABLE_OPTIMIZATION
void ASkybox::MakeTexture()
{
	int16 TextureRes = 2048;
	int32 NumStars = 50000;

	TArray<FVector2D> Points;
	for (int32 i = 0; i < NumStars; ++i)
	{
		Points.AddUnique(FVector2D(FMath::RandRange(0, TextureRes), FMath::RandRange(0, TextureRes)));
	}
	
	FString PackagePath = TEXT("/Game/ProceduralTextures");
	FString AssetName = TEXT("Skybox");
	FString LongPackageName = PackagePath + TEXT("/") + AssetName;

	IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// Ensure unique naming conventions to avoid accidental overwrites
	FString UniquePackageName;
	FString UniqueAssetName;
	AssetTools.CreateUniqueAssetName(LongPackageName, TEXT(""), UniquePackageName, UniqueAssetName);

	UPackage* Package = CreatePackage(*UniquePackageName);
	if (!Package) return;

	UTexture2D* SkyboxTexture = NewObject<UTexture2D>(Package, UTexture2D::StaticClass(), *UniqueAssetName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	SkyboxTexture->AddToRoot();								// This line prevents garbage collection of the texture
	FTexturePlatformData* NewPlatformData = new FTexturePlatformData();	// Initialize the PlatformData
	SkyboxTexture->SetPlatformData(NewPlatformData);
	NewPlatformData->SizeX = TextureRes;
	NewPlatformData->SizeY = TextureRes;
	NewPlatformData->SetNumSlices(1);
	NewPlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	SkyboxTexture->AddressX = TA_Wrap;
	SkyboxTexture->AddressY = TA_Wrap;

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
			Pixels[4 * curPixelIndex + 3] = FMath::RandRange(0, 255);
		}
	}

	// Allocate first mipmap.
	FTexture2DMipMap* Mip = new FTexture2DMipMap();
	NewPlatformData->Mips.Add(Mip);
	Mip->SizeX = TextureRes;
	Mip->SizeY = TextureRes;

	// Lock the texture so it can be modified
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*)Mip->BulkData.Realloc(TextureRes * TextureRes * 4);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * TextureRes * TextureRes * 4);
	Mip->BulkData.Unlock();

	SkyboxTexture->Source.Init(TextureRes, TextureRes, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);

	SkyboxTexture->UpdateResource();
    
	if (SkyboxTexture)
	{
		// ~ Modify asset property structures here if needed ~
		SkyboxTexture->MarkPackageDirty();

		// Broadcast structural changes to the internal Asset Registry layout
		FAssetRegistryModule::AssetCreated(SkyboxTexture);

		// Write the active package tracking system structure cleanly onto local disk
		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.SaveFlags = SAVE_NoError;

		FString PackageFileName = FPackageName::LongPackageNameToFilename(UniquePackageName, FPackageName::GetAssetPackageExtension());
        
		UPackage::Save(Package, SkyboxTexture, *PackageFileName, SaveArgs);
	}

	delete[] Pixels; // Remember to free memory before we're done
	
	DynamicMaterial->SetTextureParameterValue("StarTexture", SkyboxTexture);
}
UE_ENABLE_OPTIMIZATION
#endif


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
