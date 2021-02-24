// This is a copyright notice


#include "GasGiantColorSettings.h"
#include "AssetRegistryModule.h"
#include "Materials/MaterialInstanceDynamic.h"



UGasGiantColorSettings::UGasGiantColorSettings()
{

}

UTexture2D* UGasGiantColorSettings::MakeVoronoiTexture()
{
	TArray<FVector2D> Points;
	for (int32 i = 0; i < NumStorms; ++i)
	{
		Points.AddUnique(FVector2D(FMath::RandRange(LowBound, HighBound), FMath::RandRange(LowBound, HighBound)));
	}

	FString TextureName = "Voronoi";
	FString PackageName = TEXT("/Game/ProceduralTextures/" + this->GetName() + "_" + TextureName);
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
	FTexture2DMipMap* Mip = new(VoronoiTexture->PlatformData->Mips) FTexture2DMipMap();
	Mip->SizeX = 1024;
	Mip->SizeY = VoronoiTexture->PlatformData->SizeY;

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
