// This is a copyright notice


#include "AsteroidManager.h"
#include "AssetRegistryModule.h"
#include "Engine\Texture2DArray.h"


// Sets default values
AAsteroidManager::AAsteroidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAsteroidManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAsteroidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AAsteroidManager::NewVariants()
{
	//~ Begin making the Texture2DArray asset
	FString AssetPath = FString("/Game/ProceduralTextures/Asteroids/" + GetWorld()->GetName() + "/");
	FString AssetName = FString(TEXT("T2DA_HeightmapsArray"));
	FString PackagePath = AssetPath + AssetName;

	UPackage *Package = CreatePackage(*PackagePath);
	HeightmapsArray = NewObject<UTexture2DArray>(Package, UTexture2DArray::StaticClass(), *AssetName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);

	FAssetRegistryModule::AssetCreated(HeightmapsArray);
	HeightmapsArray->MarkPackageDirty();

	FString FilePath = FString::Printf(TEXT("%s%s%s"), *AssetPath, *AssetName, *FPackageName::GetAssetPackageExtension());
	bool bSuccess = UPackage::SavePackage(Package, HeightmapsArray, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *FilePath);
	UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), bSuccess ? TEXT("Success") : TEXT("FAIL"));
	//~ End making the Texture2DArray asset

	// Make the heightmaps
	Heightmaps.Empty();
	for (int32 i = 0; i < NumVariants; i++)
	{
		FString Name = "Heightmap_";
		Name.Append(FString::FromInt(i + 1));
		Heightmaps.EmplaceAt(i, CreateTexture(Name));
	}

	// Add the newly created heightmaps to the Texture2DArray
	HeightmapsArray->SourceTextures = Heightmaps;
	HeightmapsArray->ReloadConfig();
}


UTexture2D* AAsteroidManager::CreateTexture(FString TextureName)
{
	const int Resolution = 1024;

	FString PackageName = TEXT("/Game/ProceduralTextures/Asteroids/" + GetWorld()->GetName() + "/T_" + TextureName);
	UPackage* Package = CreatePackage(*PackageName);
	Package->FullyLoad();

	UTexture2D* NewTexture = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	NewTexture->AddToRoot();								// This line prevents garbage collection of the texture
	NewTexture->PlatformData = new FTexturePlatformData();	// Initialize the PlatformData
	NewTexture->PlatformData->SizeX = Resolution;
	NewTexture->PlatformData->SizeY = Resolution;
	NewTexture->PlatformData->SetNumSlices(1);
	NewTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	NewTexture->AddressX = TA_Clamp;
	NewTexture->AddressY = TA_Clamp;

	uint8* Pixels = new uint8[NewTexture->PlatformData->SizeX * NewTexture->PlatformData->SizeY * 4];
	for (int32 y = 0; y < Resolution; y++)
	{
		for (int32 x = 0; x < Resolution; x++)
		{
			// Pixel color here
			float color = FMath::Lerp<float>(255, -256, FMath::Pow(FVector::Dist(FVector(x, y, 0), FVector(512, 512, 0)), .8f) / (Resolution * 2));

			//FVector2D CurPixel = FVector2D(x, y);
			//FVector color = PointOnSphere(CurPixel);

			int32 curPixelIndex = ((y * Resolution) + x);
			Pixels[4 * curPixelIndex] = color;
			Pixels[4 * curPixelIndex + 1] = color;
			Pixels[4 * curPixelIndex + 2] = color;
			Pixels[4 * curPixelIndex + 3] = 255;
		}
	}

	// Allocate first mipmap.
	FTexture2DMipMap* Mip = new FTexture2DMipMap();
	NewTexture->PlatformData->Mips.Add(Mip);
	Mip->SizeX = Resolution;
	Mip->SizeY = Resolution;

	// Lock the texture so it can be modified
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*)Mip->BulkData.Realloc(NewTexture->PlatformData->SizeX * NewTexture->PlatformData->SizeY * 4);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * NewTexture->PlatformData->SizeX * NewTexture->PlatformData->SizeY * 4);
	Mip->BulkData.Unlock();

	NewTexture->Source.Init(Resolution, NewTexture->PlatformData->SizeY, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);

	NewTexture->UpdateResource();
	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(NewTexture);

	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	bool bSaved = UPackage::SavePackage(Package, NewTexture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

	delete[] Pixels;	// Don't forget to free the memory here

	return NewTexture;
}


FVector AAsteroidManager::PointOnSphere(FVector2D point)
{
	//int i = point.X + point.Y * 1024;
	//FVector2D percent = point / (1023);
	//FVector pointOnUnitCube = -LocalUp + (percent.X - .5f) * 2;
	//FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
	return FVector::ZeroVector;
}

