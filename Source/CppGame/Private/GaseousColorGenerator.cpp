// This is a copyright notice


#include "GaseousColorGenerator.h"
#include "AssetRegistryModule.h"
#include "Curves/CurveLinearColor.h"

GaseousColorGenerator::GaseousColorGenerator(UObject* owner)
{
	Owner = owner;
}

GaseousColorGenerator::~GaseousColorGenerator()
{
}

UTexture2D* GaseousColorGenerator::CreateTexture(FString TextureName, UCurveLinearColor* Gradient)
{
	FString PackageName = TEXT("/Game/ProceduralTextures/" + Owner->GetName() + "_" + TextureName);

	UPackage* Package = CreatePackage(*PackageName);
	Package->FullyLoad();

	UTexture2D* NewTexture = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	NewTexture->AddToRoot();								// This line prevents garbage collection of the texture
	NewTexture->PlatformData = new FTexturePlatformData();	// Initialize the PlatformData
	NewTexture->PlatformData->SizeX = 256;
	NewTexture->PlatformData->SizeY = 256;
	NewTexture->PlatformData->SetNumSlices(1);
	NewTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	NewTexture->AddressX = TA_Clamp;
	NewTexture->AddressY = TA_Clamp;

	uint8* Pixels = new uint8[NewTexture->PlatformData->SizeX * NewTexture->PlatformData->SizeY * 4];
	for (int32 y = 0; y < NewTexture->PlatformData->SizeY; y++)
	{
		for (int32 x = 0; x < 256; x++)
		{
			float time = (float)y / 256.f;
			FColor gradientCol = Gradient->GetLinearColorValue(time).ToFColor(true);
			int32 curPixelIndex = ((y * 256) + x);
			Pixels[4 * curPixelIndex] = gradientCol.B;
			Pixels[4 * curPixelIndex + 1] = gradientCol.G;
			Pixels[4 * curPixelIndex + 2] = gradientCol.R;
			Pixels[4 * curPixelIndex + 3] = gradientCol.A;
		}
	}

	// Allocate first mipmap.
	FTexture2DMipMap* Mip = new FTexture2DMipMap();
	NewTexture->PlatformData->Mips.Add(Mip);
	Mip->SizeX = 256;
	Mip->SizeY = 256;

	// Lock the texture so it can be modified
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*)Mip->BulkData.Realloc(NewTexture->PlatformData->SizeX * NewTexture->PlatformData->SizeY * 4);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * NewTexture->PlatformData->SizeX * NewTexture->PlatformData->SizeY * 4);
	Mip->BulkData.Unlock();

	NewTexture->Source.Init(256, 256, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);

	NewTexture->UpdateResource();
	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(NewTexture);

	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	bool bSaved = UPackage::SavePackage(Package, NewTexture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

	delete[] Pixels;	// Don't forget to free the memory here

	return NewTexture;
}

UTexture2D* GaseousColorGenerator::MakeVoronoiTexture(int8 NumStorms = 25, float StormFalloff = 2.5f, int LowBound = 100, int HighBound = 924)
{
	TArray<FVector2D> Points;
	for (int32 i = 0; i < NumStorms; ++i)
	{
		Points.AddUnique(FVector2D(FMath::RandRange(LowBound, HighBound), FMath::RandRange(LowBound, HighBound)));
	}

	FString TextureName = "Voronoi";
	FString PackageName = TEXT("/Game/ProceduralTextures/" + Owner->GetName() + "_" + TextureName);
	UPackage* Package = CreatePackage(*PackageName);
	Package->FullyLoad();

	UTexture2D* VoronoiTexture = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	VoronoiTexture->AddToRoot();								// This line prevents garbage collection of the texture
	VoronoiTexture->PlatformData = new FTexturePlatformData();	// Initialize the PlatformData
	VoronoiTexture->PlatformData->SizeX = 1024;
	VoronoiTexture->PlatformData->SizeY = 1024;
	VoronoiTexture->PlatformData->SetNumSlices(1);
	VoronoiTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	VoronoiTexture->AddressX = TA_Clamp;
	VoronoiTexture->AddressY = TA_Clamp;

	uint8* Pixels = new uint8[VoronoiTexture->PlatformData->SizeX * VoronoiTexture->PlatformData->SizeY * 4];
	for (int32 y = 0; y < VoronoiTexture->PlatformData->SizeY; y++)
	{
		for (int32 x = 0; x < 1024; x++)
		{
			// Find the closest point to the current pixel
			float PixelDist = 1024;
			FVector2D ClosePoint;
			for (auto& Point : Points)
			{
				float dist = FVector2D::Distance(FVector2D(x, y), Point);
				if (dist < PixelDist)
				{
					PixelDist = dist;
					ClosePoint = Point;
				}
			}

			// Find the closest point to that other point to determine radius of storm
			float PointDist = 1024;
			for (auto& Point : Points)
			{
				float dist = FVector2D::Distance(ClosePoint, Point);
				if (!FMath::IsNearlyEqual(dist, 0, 0.01f) && dist < PointDist)
				{
					PointDist = dist;
				}
			}
			PointDist /= StormFalloff;

			// How far away from the point is the currnet pixel
			float percent = FMath::Clamp<float>(PixelDist / PointDist, 0, 1);
			float color = FMath::Lerp<float>(255, 0, percent);

			int32 curPixelIndex = ((y * 1024) + x);
			Pixels[4 * curPixelIndex] = color;
			Pixels[4 * curPixelIndex + 1] = color;
			Pixels[4 * curPixelIndex + 2] = color;
			Pixels[4 * curPixelIndex + 3] = 255;
		}
	}

	// Allocate first mipmap.
	FTexture2DMipMap* Mip = new FTexture2DMipMap();
	VoronoiTexture->PlatformData->Mips.Add(Mip);
	Mip->SizeX = 1024;
	Mip->SizeY = 1024;

	// Lock the texture so it can be modified
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*)Mip->BulkData.Realloc(VoronoiTexture->PlatformData->SizeX * VoronoiTexture->PlatformData->SizeY * 4);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * VoronoiTexture->PlatformData->SizeX * VoronoiTexture->PlatformData->SizeY * 4);
	Mip->BulkData.Unlock();

	VoronoiTexture->Source.Init(1024, 1024, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);

	VoronoiTexture->UpdateResource();
	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(VoronoiTexture);

	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	bool bSaved = UPackage::SavePackage(Package, VoronoiTexture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

	delete[] Pixels;	// Don't forget to free the memory here

	return VoronoiTexture;
}
