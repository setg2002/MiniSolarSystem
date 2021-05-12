// Copyright Soren Gilbertson


#include "AsteroidManager.h"
#include "ShapeGenerator.h"
#include "AssetRegistryModule.h"
#include "Engine\Texture2DArray.h"
#include "Kismet/KismetMathLibrary.h"



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
	bool bSuccess = UPackage::SavePackage(Package, HeightmapsArray, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *FilePath);
	UE_LOG(LogTemp, Warning, TEXT("Saved Package: %s"), bSuccess ? TEXT("Success") : TEXT("FAIL"));
	//~ End making the Texture2DArray asset
	
	// Make the sphere offset texture
	//CreateSphereTexture("AsteroidSphere");
	
#if WITH_EDITOR
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


UTexture2D* AAsteroidManager::CreateHeightmapTexture(FString TextureName)
{
	const int Resolution = 1024;

	UTexture2D* DynamicTexture = UTexture2D::CreateTransient(Resolution, Resolution, EPixelFormat::PF_B8G8R8A8);

	DynamicTexture->UpdateResource();

	float seed = FMath::FRand() * 1000;

	uint8* Pixels = new uint8[Resolution * Resolution * 4];
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

	FUpdateTextureRegion2D* Region = new FUpdateTextureRegion2D;
	Region->DestX = 0;
	Region->DestY = 0;
	Region->SrcX = 0;
	Region->SrcY = 0;
	Region->Width = Resolution;
	Region->Height = Resolution;

	TFunction<void(uint8* SrcData, const FUpdateTextureRegion2D* Regions)> DataCleanupFunc =
		[](uint8* SrcData, const FUpdateTextureRegion2D* Regions) {
		delete[] SrcData;
		delete[] Regions;
	};

	DynamicTexture->UpdateTextureRegions(0, 1, Region, Resolution * 4, 4, Pixels);

	return DynamicTexture;
}


UTexture2D* AAsteroidManager::CreateSphereTexture(FString TextureName)
{
	const int Resolution = 1024 * 4;

	UTexture2D* DynamicTexture = UTexture2D::CreateTransient(Resolution, Resolution, EPixelFormat::PF_B8G8R8A8);

	DynamicTexture->UpdateResource();

	uint8* Pixels = new uint8[Resolution * Resolution * 4];
	for (int32 y = 0; y < Resolution; y++)
	{
		for (int32 x = 0; x < Resolution; x++)
		{
			FVector2D CurUnitPixel = FVector2D(float(x) / float(Resolution), float(y) / float(Resolution));
			FVector color = PointOnUnitSphere(CurUnitPixel) * 255;

			int32 curPixelIndex = ((y * Resolution) + x);
			Pixels[4 * curPixelIndex    ] = color.Z;
			Pixels[4 * curPixelIndex + 1] = color.Y;
			Pixels[4 * curPixelIndex + 2] = color.X;
			Pixels[4 * curPixelIndex + 3] = /*color == 0 ? 0 : */255;
		}
	}

	FUpdateTextureRegion2D* Region = new FUpdateTextureRegion2D;
	Region->DestX = 0;
	Region->DestY = 0;
	Region->SrcX = 0;
	Region->SrcY = 0;
	Region->Width = Resolution;
	Region->Height = Resolution;

	TFunction<void(uint8* SrcData, const FUpdateTextureRegion2D* Regions)> DataCleanupFunc =
		[](uint8* SrcData, const FUpdateTextureRegion2D* Regions) {
		delete[] SrcData;
		delete[] Regions;
	};

	DynamicTexture->UpdateTextureRegions(0, 1, Region, Resolution * 4, 4, Pixels);

	return DynamicTexture;
}


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
