// This is a copyright notice


#include "AsteroidManager.h"
#include "AssetRegistryModule.h"
#include "Engine\Texture2DArray.h"
#include "Kismet/KismetMathLibrary.h"



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
			//float color = FMath::Lerp<float>(255, -256, FMath::Pow(FVector::Dist(FVector(x, y, 0), FVector(512, 512, 0)), e) / (Resolution * 2));

			FVector2D CurUnitPixel = FVector2D(float(x) / float(Resolution), float(y) / float(Resolution));
			FVector color = PointOnSphere(CurUnitPixel) * 255;

			int32 curPixelIndex = ((y * Resolution) + x);
			Pixels[4 * curPixelIndex] = color.Z;
			Pixels[4 * curPixelIndex + 1] = color.Y;
			Pixels[4 * curPixelIndex + 2] = color.X;
			Pixels[4 * curPixelIndex + 3] = color == FVector::ZeroVector ? 0 : 255;
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


FVector AAsteroidManager::PointOnSphere(FVector2D pointOnUnitSquare)
{
	static const TArray<FVector> LocalUps = { FVector(0, 1, 0), FVector(0, 0, -1), FVector(1, 0, 0), FVector(0, 0, 1), FVector(0, -1, 0), FVector(-1, 0, 0) };
	static const TArray<TArray<FVector2D>> Coordinates = {
		{ FVector2D(0.375, 0),    FVector2D(0.625, 0.25) },
		{ FVector2D(0.125, 0.25), FVector2D(0.375, 0.5) },
		{ FVector2D(0.375, 0.25), FVector2D(0.625, 0.5) },
		{ FVector2D(0.625, 0.25), FVector2D(0.875, 0.5) },
		{ FVector2D(0.375, 0.5),  FVector2D(0.625, 0.75) },
		{ FVector2D(0.375, 0.75), FVector2D(0.625, 1.f) }
	};

	// Find coresponding face 
	int8 faceIndex = 6;

	for (int8 i = 0; i < 6; i++)
	{
		if (IsPointWithinFace(pointOnUnitSquare, i))
		{
			faceIndex = i;
			break;
		}
	}
	if (faceIndex == 6) // pointOnUnitSquare is not in the UVs
	{
		return FVector::ZeroVector;
	}
	
	//LocalUps[faceIndex] * 255/* / 2 + .5f*/;
	return FVector(FMath::Abs(LocalUps[faceIndex].X), FMath::Abs(LocalUps[faceIndex].Y), FMath::Abs(LocalUps[faceIndex].Z));

	//FVector LocalUp = LocalUps[faceIndex];
	//FVector axisA = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X);
	//FVector axisB = FVector().CrossProduct(LocalUp, axisA);

	//FVector2D percent = pointOnUnitSquare * Coordinates[faceIndex][1];
	//return 255 * LocalUp * FMath::Lerp<float>(1, -1, FMath::Pow(FVector::Dist(FVector(percent.X, percent.Y, 0), FVector(.5, .5, 0)), e) / (FMath::Sqrt(2)));
	
	//FVector pointOnUnitCube = -LocalUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
	//FVector pointOnUnitSphere = pointOnUnitCube.GetSafeNormal();
	//return pointOnUnitCube * 255;
}


bool AAsteroidManager::IsPointWithinFace(FVector2D pointToTest, int8 faceToTest)
{
	static const TArray<TArray<FVector2D>> Coordinates = { 
		{ FVector2D(0.375, 0),    FVector2D(0.625, 0.25) }, 
		{ FVector2D(0.125, 0.25), FVector2D(0.375, 0.5) },
		{ FVector2D(0.375, 0.25), FVector2D(0.625, 0.5) },
		{ FVector2D(0.625, 0.25), FVector2D(0.875, 0.5) },
		{ FVector2D(0.375, 0.5),  FVector2D(0.625, 0.75) },
		{ FVector2D(0.375, 0.75), FVector2D(0.625, 1.f) }
	};

	if (pointToTest.X >= Coordinates[faceToTest][0].X &&
		pointToTest.X <= Coordinates[faceToTest][1].X &&
		pointToTest.Y >= Coordinates[faceToTest][0].Y &&
		pointToTest.Y <= Coordinates[faceToTest][1].Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}
