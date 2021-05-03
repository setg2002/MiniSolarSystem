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
	const int16 sizeX = 256;
	const int16 sizeY = 1;

	UTexture2D* DynamicTexture = UTexture2D::CreateTransient(sizeX, sizeY, EPixelFormat::PF_B8G8R8A8);

	DynamicTexture->UpdateResource();

	uint8* Pixels = new uint8[sizeX * sizeY * 4];
	for (int16 y = 0; y < sizeY; y++)
	{
		for (int16 x = 0; x < sizeX; x++)
		{
			float time = (float)x / 256.f;
			FColor gradientCol = Gradient->GetLinearColorValue(time).ToFColor(true);
			int32 curPixelIndex = ((y * sizeY) + x);
			Pixels[4 * curPixelIndex] = gradientCol.B;
			Pixels[4 * curPixelIndex + 1] = gradientCol.G;
			Pixels[4 * curPixelIndex + 2] = gradientCol.R;
			Pixels[4 * curPixelIndex + 3] = gradientCol.A;
		}
	}

	FUpdateTextureRegion2D* Region = new FUpdateTextureRegion2D;
	Region->DestX = 0;
	Region->DestY = 0;
	Region->SrcX = 0;
	Region->SrcY = 0;
	Region->Width = sizeX;
	Region->Height = sizeY;

	TFunction<void(uint8* SrcData, const FUpdateTextureRegion2D* Regions)> DataCleanupFunc =
		[](uint8* SrcData, const FUpdateTextureRegion2D* Regions) {
		delete[] SrcData;
		delete[] Regions;
	};

	DynamicTexture->UpdateTextureRegions(0, 1, Region, sizeX * 4, 4, Pixels);

	return DynamicTexture;
}

UTexture2D* GaseousColorGenerator::MakeVoronoiTexture(int16 NumStorms, float StormFalloff, int LowBound, int HighBound, int TextureResolution)
{
	// Make sure TextureResolution is a power of 2
	FGenericPlatformMath::FloorLog2(FMath::Max(TextureResolution, 1) * 2);

	TArray<FVector2D> Points;
	for (int32 i = 0; i < NumStorms; ++i)
	{
		Points.AddUnique(FVector2D(FMath::RandRange(LowBound, HighBound), FMath::RandRange(LowBound, HighBound)));
	}

	// Precalculate closest points to one another
	TMap<FVector2D, float> StormRadii;
	
	for (auto& Point : Points)
	{
		StormRadii.Add(Point);
	}
	for (auto It = StormRadii.CreateConstIterator(); It; ++It)
	{
		float radius = 1024;
		for (auto& Point : Points)
		{
			float dist = FVector2D::Distance(Point, It.Key());
			if (Point != It.Key() && dist < radius)
			{
				radius = dist;
			}
		}
		radius /= StormFalloff;
		StormRadii.Emplace(It.Key(), radius);
	}

	UTexture2D* DynamicTexture = UTexture2D::CreateTransient(TextureResolution, TextureResolution, EPixelFormat::PF_B8G8R8A8);

	DynamicTexture->UpdateResource();

	uint8* Pixels = new uint8[TextureResolution * TextureResolution * 4];
	for (int32 y = 0; y < TextureResolution; y++)
	{
		for (int32 x = 0; x < TextureResolution; x++)
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
			float PointDist = StormRadii[ClosePoint];

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

	FUpdateTextureRegion2D* Region = new FUpdateTextureRegion2D;
	Region->DestX = 0;
	Region->DestY = 0;
	Region->SrcX = 0;
	Region->SrcY = 0;
	Region->Width = TextureResolution;
	Region->Height = TextureResolution;

	TFunction<void(uint8* SrcData, const FUpdateTextureRegion2D* Regions)> DataCleanupFunc =
		[](uint8* SrcData, const FUpdateTextureRegion2D* Regions) {
		delete[] SrcData;
		delete[] Regions;
	};

	DynamicTexture->UpdateTextureRegions(0, 1, Region, TextureResolution * 4, 4, Pixels);

	return DynamicTexture;
}
