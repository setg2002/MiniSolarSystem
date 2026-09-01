// Copyright Soren Gilbertson


#include "AsteroidManager.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Shape/ShapeGenerator.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine\Texture2DArray.h"
#include "UObject/SavePackage.h"


// Sets default values
AAsteroidManager::AAsteroidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	shapeGenerator = new ShapeGenerator();
}

// Called when the game starts or when spawned
void AAsteroidManager::BeginPlay()
{
	Super::BeginPlay();
	
}


void AAsteroidManager::NewVariants()
{
	shapeGenerator->UpdateSettings(ShapeSettings);

	//~ Begin making the Texture2DArray asset
	FString AssetPath = FString("/Game/ProceduralTextures/Asteroids/" + GetWorld()->GetName() + "/");
	FString AssetName = FString(TEXT("T2DA_HeightmapsArray"));
	FString PackagePath = AssetPath + AssetName;

	UPackage *Package = CreatePackage(*PackagePath);
	HeightmapsArray = NewObject<UTexture2DArray>(Package, UTexture2DArray::StaticClass(), *AssetName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);

	FAssetRegistryModule::AssetCreated(HeightmapsArray);
	HeightmapsArray->MarkPackageDirty();

	FString FilePath = FString::Printf(TEXT("%s%s%s"), *AssetPath, *AssetName, *FPackageName::GetAssetPackageExtension());
	FSavePackageArgs Args = FSavePackageArgs();
	Args.SaveFlags = SAVE_NoError;
	Args.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
	bool bSuccess = UPackage::SavePackage(Package, HeightmapsArray, *FilePath, Args);
	UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), bSuccess ? TEXT("Success") : TEXT("FAIL"));
	//~ End making the Texture2DArray asset

#if WITH_EDITOR	
	// Make the sphere offset texture
	if (bMakeSphere)
	{
		CreateSphereTexture("AsteroidSphere");
	}

	// Make the heightmaps
	HeightmapsArray->SourceTextures.Empty();
	HeightmapsArray->SourceTextures.SetNum(NumVariants);
	for (int32 i = 0; i < NumVariants; i++)
	{
		FString Name = "Heightmap_";
		Name.Append(FString::FromInt(i + 1));
		HeightmapsArray->SourceTextures[i] = CreateHeightmapTexture(Name);
	}
	//HeightmapsArray->ReloadConfig(UTexture2DArray::StaticClass());  // Why aren't the new heightmaps showing up?
#endif
}


#if WITH_EDITOR
UTexture2D* AAsteroidManager::CreateHeightmapTexture(FString TextureName)
{
	const int Resolution = 1024;
	
	FString PackagePath = TEXT("/Game/ProceduralTextures/Asteroids");
	FString AssetName = TEXT("T_" + TextureName);
	FString LongPackageName = PackagePath + TEXT("/") + AssetName;

	IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// Ensure unique naming conventions to avoid accidental overwrites
	FString UniquePackageName;
	FString UniqueAssetName;
	AssetTools.CreateUniqueAssetName(LongPackageName, TEXT(""), UniquePackageName, UniqueAssetName);

	UPackage* Package = CreatePackage(*UniquePackageName);
	if (!Package)
	{
		return nullptr;
	}

	UTexture2D* NewTexture = NewObject<UTexture2D>(Package, UTexture2D::StaticClass(), *UniqueAssetName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	NewTexture->AddToRoot();								// This line prevents garbage collection of the texture
	FTexturePlatformData* NewPlatformData = new FTexturePlatformData();	// Initialize the PlatformData
	NewTexture->SetPlatformData(NewPlatformData);
	NewPlatformData->SizeX = Resolution;
	NewPlatformData->SizeY = Resolution;
	NewPlatformData->SetNumSlices(1);
	NewPlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	NewTexture->AddressX = TA_Clamp;
	NewTexture->AddressY = TA_Clamp;

	float seed = FMath::FRand() * 1000;

	uint8* Pixels = new uint8[NewPlatformData->SizeX * NewPlatformData->SizeY * 4];
	for (int32 y = 0; y < Resolution; y++)
	{
		for (int32 x = 0; x < Resolution; x++)
		{
			FVector2D Pixel = FVector2D(float(x), float(y));
			float color = shapeGenerator->CalculateUnscaledElevation(FVector(Pixel, seed)) * 255;

			int32 curPixelIndex = ((y * Resolution) + x);
			Pixels[4 * curPixelIndex] = color;
			Pixels[4 * curPixelIndex + 1] = color;
			Pixels[4 * curPixelIndex + 2] = color;
			Pixels[4 * curPixelIndex + 3] = 255;
		}
	}

	// Allocate first mipmap.
	FTexture2DMipMap* Mip = new FTexture2DMipMap();
	NewPlatformData->Mips.Add(Mip);
	Mip->SizeX = Resolution;
	Mip->SizeY = Resolution;

	// Lock the texture so it can be modified
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*)Mip->BulkData.Realloc(NewPlatformData->SizeX * NewPlatformData->SizeY * 4);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * NewPlatformData->SizeX * NewPlatformData->SizeY * 4);
	Mip->BulkData.Unlock();
	
	NewTexture->Source.Init(Resolution, NewPlatformData->SizeY, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);

	NewTexture->UpdateResource();
	
	if (NewTexture)
	{
		// ~ Modify asset property structures here if needed ~
		NewTexture->MarkPackageDirty();

		// Broadcast structural changes to the internal Asset Registry layout
		FAssetRegistryModule::AssetCreated(NewTexture);

		// Write the active package tracking system structure cleanly onto local disk
		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.SaveFlags = SAVE_NoError;

		FString PackageFileName = FPackageName::LongPackageNameToFilename(UniquePackageName, FPackageName::GetAssetPackageExtension());
        
		UPackage::Save(Package, NewTexture, *PackageFileName, SaveArgs);
	}

	delete[] Pixels;	// Don't forget to free the memory here

	return NewTexture;
}


UTexture2D* AAsteroidManager::CreateSphereTexture(FString TextureName)
{
	const int Resolution = 1024 * 4;

	FString PackagePath = TEXT("/Game/ProceduralTextures/Asteroids");
	FString AssetName = TEXT("T_" + TextureName);
	FString LongPackageName = PackagePath + TEXT("/") + AssetName;

	IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// Ensure unique naming conventions to avoid accidental overwrites
	FString UniquePackageName;
	FString UniqueAssetName;
	AssetTools.CreateUniqueAssetName(LongPackageName, TEXT(""), UniquePackageName, UniqueAssetName);

	UPackage* Package = CreatePackage(*UniquePackageName);
	if (!Package)
	{
		return nullptr;
	}

	UTexture2D* NewTexture = NewObject<UTexture2D>(Package, UTexture2D::StaticClass(), *UniqueAssetName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	NewTexture->AddToRoot();								// This line prevents garbage collection of the texture
	FTexturePlatformData* NewPlatformData = new FTexturePlatformData();	// Initialize the PlatformData
	NewTexture->SetPlatformData(NewPlatformData);
	NewPlatformData->SizeX = Resolution;
	NewPlatformData->SizeY = Resolution;
	NewPlatformData->SetNumSlices(1);
	NewPlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	NewTexture->AddressX = TA_Clamp;
	NewTexture->AddressY = TA_Clamp;

	uint8* Pixels = new uint8[NewPlatformData->SizeX * NewPlatformData->SizeY * 4];
	for (int32 y = 0; y < Resolution; y++)
	{
		for (int32 x = 0; x < Resolution; x++)
		{
			FVector2D CurUnitPixel = FVector2D(float(x) / float(Resolution), float(y) / float(Resolution));
			FVector color = PointOnUnitSphere(CurUnitPixel) * 255;

			int32 curPixelIndex = ((y * Resolution) + x);
			Pixels[4 * curPixelIndex] = color.Z;
			Pixels[4 * curPixelIndex + 1] = color.Y;
			Pixels[4 * curPixelIndex + 2] = color.X;
			Pixels[4 * curPixelIndex + 3] = /*color == 0 ? 0 : */255;
		}
	}

	// Allocate first mipmap.
	FTexture2DMipMap* Mip = new FTexture2DMipMap();
	NewPlatformData->Mips.Add(Mip);
	Mip->SizeX = Resolution;
	Mip->SizeY = Resolution;

	// Lock the texture so it can be modified
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*)Mip->BulkData.Realloc(NewPlatformData->SizeX * NewPlatformData->SizeY * 4);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * NewPlatformData->SizeX * NewPlatformData->SizeY * 4);
	Mip->BulkData.Unlock();
	
	NewTexture->Source.Init(Resolution, NewPlatformData->SizeY, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);

	NewTexture->UpdateResource();

	if (NewTexture)
	{
		// ~ Modify asset property structures here if needed ~
		NewTexture->MarkPackageDirty();

		// Broadcast structural changes to the internal Asset Registry layout
		FAssetRegistryModule::AssetCreated(NewTexture);

		// Write the active package tracking system structure cleanly onto local disk
		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.SaveFlags = SAVE_NoError;

		FString PackageFileName = FPackageName::LongPackageNameToFilename(UniquePackageName, FPackageName::GetAssetPackageExtension());
        
		UPackage::Save(Package, NewTexture, *PackageFileName, SaveArgs);
	}

	delete[] Pixels;	// Don't forget to free the memory here

	return NewTexture;
}
#endif


FVector AAsteroidManager::PointOnUnitSphere(FVector2D pointOnUnitSquare)
{
	static const TArray<FVector> LocalUps = { FVector(0, 1, 0), FVector(0, 0, -1), FVector(1, 0, 0), FVector(0, 0, 1), FVector(0, -1, 0), FVector(-1, 0, 0) };

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

	FVector LocalUp = LocalUps[faceIndex];
	FVector axisA = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X);
	FVector axisB = FVector().CrossProduct(LocalUp, axisA);

	FVector2D percent = (pointOnUnitSquare - Coordinates[faceIndex][0]) / .25f;	
	
	FVector pointOnUnitCube = LocalUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
	FVector pointOnUnitSphere = pointOnUnitCube.GetUnsafeNormal();
	return pointOnUnitSphere = FVector(    // Only have do do this because Abs doesn't like to work on vectors apparently
		FMath::Abs(pointOnUnitSphere.X - pointOnUnitCube.X),
		FMath::Abs(pointOnUnitSphere.Y - pointOnUnitCube.Y),
		FMath::Abs(pointOnUnitSphere.Z - pointOnUnitCube.Z)
	);
}


bool AAsteroidManager::IsPointWithinFace(FVector2D pointToTest, int8 faceToTest)
{
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
