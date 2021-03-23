// This is a copyright notice


#include "Skybox.h"
#include "AssetRegistryModule.h"


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
	DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/Instances/M_SkyBox_Inst.M_SkyBox_Inst'"), NULL, LOAD_None, NULL));
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

void /*UTexture2D*/ ASkybox::MakeTexture()
{
	int16 TextureRes = 2048;
	int32 NumStars = 50000;

	TArray<FVector2D> Points;
	for (int32 i = 0; i < NumStars; ++i)
	{
		Points.AddUnique(FVector2D(FMath::RandRange(0, TextureRes), FMath::RandRange(0, TextureRes)));
	}

	FString TextureName = "Skybox";
	FString PackageName = TEXT("/Game/ProceduralTextures/" + this->GetName() + "_" + TextureName);
	UPackage* Package = CreatePackage(*PackageName);
	Package->FullyLoad();

	UTexture2D* SkyboxTexture = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	SkyboxTexture->AddToRoot();								// This line prevents garbage collection of the texture
	SkyboxTexture->PlatformData = new FTexturePlatformData();	// Initialize the PlatformData
	SkyboxTexture->PlatformData->SizeX = TextureRes;
	SkyboxTexture->PlatformData->SizeY = TextureRes;
	SkyboxTexture->PlatformData->SetNumSlices(1);
	SkyboxTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	SkyboxTexture->AddressX = TA_Clamp;
	SkyboxTexture->AddressY = TA_Clamp;

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

	// Allocate first mipmap.
	FTexture2DMipMap* Mip = new FTexture2DMipMap();
	SkyboxTexture->PlatformData->Mips.Add(Mip);
	Mip->SizeX = TextureRes;
	Mip->SizeY = TextureRes;

	// Lock the texture so it can be modified
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*)Mip->BulkData.Realloc(TextureRes * TextureRes * 4);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * TextureRes * TextureRes * 4);
	Mip->BulkData.Unlock();

	SkyboxTexture->Source.Init(TextureRes, TextureRes, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);

	SkyboxTexture->UpdateResource();
	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(SkyboxTexture);

	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	bool bSaved = UPackage::SavePackage(Package, SkyboxTexture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

	delete[] Pixels;	// Don't forget to free the memory here

	return /*SkyboxTexture*/;
}
