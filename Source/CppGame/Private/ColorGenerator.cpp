// This is a copyright notice


#include "ColorGenerator.h"
#include "MinMax.h"
#include "INoiseFilter.h"
#include "AssetCleaner.h"
#include "ColorSettings.h"
#include "NoiseFilterFactory.h"
#include "AssetRegistryModule.h"
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
	if (ColorSettings->BiomeColorSettings->bUsingNoise)
	{
		BiomeNoiseFilter = NoiseFilterFactory::CreateNoiseFilter(ColorSettings->BiomeColorSettings->Noise);
	}
}

TerrestrialColorGenerator::~TerrestrialColorGenerator()
{
}

void TerrestrialColorGenerator::UpdateElevation(MinMax* elevationMinMax)
{
	for (int i = 0; i < ColorSettings->DynamicMaterials.Num(); i++)
	{
		ColorSettings->DynamicMaterials[i]->SetVectorParameterValue(FName("elevationMinMax"), FLinearColor(elevationMinMax->Min, elevationMinMax->Max, 0));
	}
}

float TerrestrialColorGenerator::BiomePercentFromPoint(FVector PointOnUnitSphere)
{
	float HeightPercent = (PointOnUnitSphere.Z + 1) / 2.f;
	// Offset for using noise
	if (ColorSettings->BiomeColorSettings->bUsingNoise)
	{
		HeightPercent += (BiomeNoiseFilter->Evaluate(PointOnUnitSphere) - ColorSettings->BiomeColorSettings->NoiseOffset) * ColorSettings->BiomeColorSettings->NoiseStrength;
	}
	
	float BiomeIndex = 0;
	int NumBiomes = ColorSettings->BiomeColorSettings->Biomes.Num();
	float blendRange = ColorSettings->BiomeColorSettings->blendAmount / 2.f + 0.001f;

	for (int i = 0; i < NumBiomes; i++)
	{
		float dst = HeightPercent - ColorSettings->BiomeColorSettings->Biomes[i]->StartHeight;
		float weight = FMath::Clamp<float>(UKismetMathLibrary::NormalizeToRange(dst, -blendRange, blendRange), 0, 1);
		BiomeIndex *= (1 - weight);
		BiomeIndex += i * weight;
	}
	return (float)BiomeIndex / FMath::Max<int>(1, NumBiomes - 1);
}

#if WITH_EDITOR
void TerrestrialColorGenerator::UpdateColors()
{
	TArray<UCurveLinearColor*> biomeColors;
	for (int i = 0; i < ColorSettings->BiomeColorSettings->Biomes.Num(); i++)
	{
		biomeColors.Add(ColorSettings->BiomeColorSettings->Biomes[i]->Gradient);
	}

	UTexture2D* SurfaceTexture = CreateTexture(FString("TerrainTexture"), biomeColors);
	for (int i = 0; i < ColorSettings->DynamicMaterials.Num(); i++)
	{
		ColorSettings->DynamicMaterials[i]->SetTextureParameterValue(FName("_texture"), SurfaceTexture);
	}

	UTexture2D* OceanTexture = CreateTexture(FString("OceanTexture"), TArray<UCurveLinearColor*>() = { ColorSettings->OceanColor });
	for (int i = 0; i < ColorSettings->DynamicMaterials.Num(); i++)
	{
		ColorSettings->DynamicMaterials[i]->SetTextureParameterValue(FName("_oceanTexture"), OceanTexture);
	}

	//AssetCleaner::CleanDirectory(EDirectoryFilterType::Textures);
}


UTexture2D* TerrestrialColorGenerator::CreateTexture(FString TextureName, TArray<UCurveLinearColor*> Gradients)
{
	//TODO Make textures work with non power of two y sizes (number of gradients that aren't a power of two)

	FString PackageName = TEXT("/Game/ProceduralTextures/" + Owner->GetName() + "_" + TextureName);
	UPackage* Package = CreatePackage(*PackageName);
	Package->FullyLoad();

	UTexture2D* NewTexture = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	NewTexture->AddToRoot();								// This line prevents garbage collection of the texture
	NewTexture->PlatformData = new FTexturePlatformData();	// Initialize the PlatformData
	NewTexture->PlatformData->SizeX = TextureResolution;
	NewTexture->PlatformData->SizeY = Gradients.Num();
	NewTexture->PlatformData->SetNumSlices(1);
	NewTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	NewTexture->AddressX = TA_Clamp;
	NewTexture->AddressY = TA_Clamp;

	uint8* Pixels = new uint8[NewTexture->PlatformData->SizeX * NewTexture->PlatformData->SizeY * 4];
	for (int32 y = 0; y < NewTexture->PlatformData->SizeY; y++)
	{
		for (int32 x = 0; x < TextureResolution; x++)
		{
			float time = (float)x / 256.f;
			FColor gradientCol = Gradients[y]->GetLinearColorValue(time).ToFColor(true);
			int32 curPixelIndex = ((y * TextureResolution) + x);
			Pixels[4 * curPixelIndex] = gradientCol.B;
			Pixels[4 * curPixelIndex + 1] = gradientCol.G;
			Pixels[4 * curPixelIndex + 2] = gradientCol.R;
			Pixels[4 * curPixelIndex + 3] = gradientCol.A;
		}
	}

	// Allocate first mipmap.
	FTexture2DMipMap* Mip = new FTexture2DMipMap();
	NewTexture->PlatformData->Mips.Add(Mip);
	Mip->SizeX = TextureResolution;
	Mip->SizeY = Gradients.Num();

	// Lock the texture so it can be modified
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*)Mip->BulkData.Realloc(NewTexture->PlatformData->SizeX * NewTexture->PlatformData->SizeY * 4);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * NewTexture->PlatformData->SizeX * NewTexture->PlatformData->SizeY * 4);
	Mip->BulkData.Unlock();

	NewTexture->Source.Init(TextureResolution, NewTexture->PlatformData->SizeY, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);

	NewTexture->UpdateResource();
	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(NewTexture);

	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	bool bSaved = UPackage::SavePackage(Package, NewTexture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

	delete[] Pixels;	// Don't forget to free the memory here

	return NewTexture;
}
#endif
