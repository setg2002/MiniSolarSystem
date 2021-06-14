// Copyright Soren Gilbertson


#include "ColorCurveFunctionLibrary.h"
#include "Curves/CurveLinearColor.h"


UTexture2D* UColorCurveFunctionLibrary::TextureFromCurve(UCurveLinearColor* Gradient, int32 sizeX = 256, int32 sizeY = 1)
{
	ensure(sizeX > 0 && sizeY > 0);

	UTexture2D* DynamicTexture = UTexture2D::CreateTransient(sizeX, sizeY, EPixelFormat::PF_B8G8R8A8);

	DynamicTexture->UpdateResource();

	uint8* Pixels = new uint8[sizeX * sizeY * 4];
	for (int32 y = 0; y < sizeY; y++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			float time = (float)x / (float)sizeX;
			FColor gradientCol = Gradient->GetLinearColorValue(time).ToFColor(true);
			int32 curPixelIndex = ((y * sizeX) + x);
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
