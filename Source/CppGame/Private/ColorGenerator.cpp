// Copyright Soren Gilbertson


#include "ColorGenerator.h"
#include "MinMax.h"
#include "CoreMinimal.h"
#include "INoiseFilter.h"
#include "ColorSettings.h"
#include "NoiseFilterFactory.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveLinearColor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"


TerrestrialColorGenerator::TerrestrialColorGenerator(AActor* owner)
{
	Owner = owner;
}

void TerrestrialColorGenerator::UpdateSettings(UColorSettings* colorSettings)
{
	this->ColorSettings = colorSettings;
	if (ColorSettings->BiomeColorSettings->GetUsingNoise())
	{
		BiomeNoiseFilter = NoiseFilterFactory::CreateNoiseFilter(ColorSettings->BiomeColorSettings->GetNoise());
	}
}

TerrestrialColorGenerator::~TerrestrialColorGenerator()
{
}

void TerrestrialColorGenerator::UpdateElevation(MinMax* elevationMinMax)
{
	ColorSettings->DynamicMaterial->SetVectorParameterValue(FName("elevationMinMax"), FLinearColor(elevationMinMax->Min, elevationMinMax->Max, 0));
}

float TerrestrialColorGenerator::BiomePercentFromPoint(FVector PointOnUnitSphere)
{
	float HeightPercent = (PointOnUnitSphere.Z + 1) / 2.f;
	// Offset for using noise
	if (ColorSettings->BiomeColorSettings->GetUsingNoise())
	{
		HeightPercent += (BiomeNoiseFilter->Evaluate(PointOnUnitSphere) - ColorSettings->BiomeColorSettings->GetNoiseOffset()) * ColorSettings->BiomeColorSettings->GetNoiseStrength();
	}
	
	float BiomeIndex = 0;
	int NumBiomes = ColorSettings->BiomeColorSettings->GetBiomes().Num();
	float blendRange = ColorSettings->BiomeColorSettings->GetBlendAmount() / 2.f + 0.001f;

	for (int i = 0; i < NumBiomes; i++)
	{
		float dst = HeightPercent - ColorSettings->BiomeColorSettings->GetBiomes()[i]->GetStartHeight();
		float weight = FMath::Clamp<float>(UKismetMathLibrary::NormalizeToRange(dst, -blendRange, blendRange), 0, 1);
		BiomeIndex *= (1 - weight);
		BiomeIndex += i * weight;
	}
	return (float)BiomeIndex / FMath::Max<int>(1, NumBiomes - 1);
}


void TerrestrialColorGenerator::UpdateColors()
{
	TArray<UCurveLinearColor*> biomeColors;
	ensure(ColorSettings);
	for (int i = 0; i < ColorSettings->BiomeColorSettings->GetBiomes().Num(); i++)
	{
		biomeColors.Add(ColorSettings->BiomeColorSettings->GetBiomes()[i]->Gradient);
	}

	UTexture2D* SurfaceTexture = CreateTexture(FString("TerrainTexture"), biomeColors);
	ColorSettings->DynamicMaterial->SetTextureParameterValue(FName("_texture"), SurfaceTexture);

	UTexture2D* OceanTexture = CreateTexture(FString("OceanTexture"), TArray<UCurveLinearColor*>() = { ColorSettings->OceanColor });
	ColorSettings->DynamicMaterial->SetTextureParameterValue(FName("_oceanTexture"), OceanTexture);
	
	//AssetCleaner::CleanDirectory(EDirectoryFilterType::Textures);
}


UTexture2D* TerrestrialColorGenerator::CreateTexture(FString TextureName, TArray<UCurveLinearColor*> Gradients)
{
	UTexture2D* DynamicTexture = UTexture2D::CreateTransient(TextureResolution, Gradients.Num(), EPixelFormat::PF_B8G8R8A8);
	
	DynamicTexture->UpdateResource(); // Absence of this line (or moving it to after UpdateTextureRegions()) will result in a corrupt looking texture, pretty cool

	uint8* Pixels = new uint8[TextureResolution * Gradients.Num() * 4];
	for (int32 y = 0; y < Gradients.Num(); y++)
	{
		for (int32 x = 0; x < TextureResolution; x++)
		{
			float time = (float)x / 256.f;
			FColor gradientCol = Gradients[y]->GetLinearColorValue(time).ToFColor(true);
			int32 curPixelIndex = ((y * TextureResolution) + x);
			Pixels[4 * curPixelIndex    ] = gradientCol.B;
			Pixels[4 * curPixelIndex + 1] = gradientCol.G;
			Pixels[4 * curPixelIndex + 2] = gradientCol.R;
			Pixels[4 * curPixelIndex + 3] = gradientCol.A;
		}
	}

	FUpdateTextureRegion2D* Region = new FUpdateTextureRegion2D;
	Region->DestX = 0;
	Region->DestY = 0;
	Region->SrcX  = 0;
	Region->SrcY  = 0;
	Region->Width = TextureResolution;
	Region->Height = Gradients.Num();

	TFunction<void(uint8* SrcData, const FUpdateTextureRegion2D* Regions)> DataCleanupFunc =
		[](uint8* SrcData, const FUpdateTextureRegion2D* Regions) {
		delete[] SrcData;
		delete[] Regions;
	};

	DynamicTexture->UpdateTextureRegions(0, 1, Region, TextureResolution * 4, 4, Pixels);

	return DynamicTexture;
}

