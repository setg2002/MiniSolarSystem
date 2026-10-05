// Copyright Soren Gilbertson


#include "Color/ColorCurveFunctionLibrary.h"
#include "Game/CelestialSaveGameArchive.h"
#include "Curves/CurveLinearColor.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"


UTexture2D* UColorCurveFunctionLibrary::TextureFromCurve(UCurveLinearColor* Gradient, int32 sizeX = 256, int32 sizeY = 1)
{
	ensure(sizeX > 0 && sizeY > 0 && Gradient != nullptr);

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

UCurveLinearColor* UColorCurveFunctionLibrary::CreateNewCurve(FName Name, TArray<uint8> Data)
{
	FString PackageName = TEXT("/Game/Materials/Gradients/Runtime/");
	UPackage* Package = CreatePackage(*PackageName);

	UCurveLinearColor* NewGradient =  NewObject<UCurveLinearColor>(Package, Name, EObjectFlags::RF_Public);

	if (NewGradient != NULL && Data.Num() > 1)
	{
		// Fill in the asset's data here
		FMemoryReader MemoryReader(Data);
		FCelestialSaveGameArchive Ar(MemoryReader);
		NewGradient->Serialize(Ar);
	}

	FAssetRegistryModule::AssetCreated(NewGradient);
	NewGradient->MarkPackageDirty();
	FString AssetName = Name.ToString();
	FString FilePath = FString::Printf(TEXT("%s%s%s"), *PackageName, *AssetName, *FPackageName::GetAssetPackageExtension());
	FSavePackageArgs Args = FSavePackageArgs();
	Args.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
	UPackage::SavePackage(Package, NewGradient, *FilePath, Args);
	return NewGradient;
}

UCurveLinearColor* UColorCurveFunctionLibrary::CreateRandomCurve(int32 NumPoints, bool bClampColorDelta, uint8 MaxDelta)
{
	if (NumPoints <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("UColorCurveFunctionLibrary::CreateRandomCurve: Cannot create a curve with non-positive number of points"));
		return nullptr;
	}
	
	UCurveLinearColor* NewGradient = CreateNewCurve(FName(FString::Printf(TEXT("RandomGradient_%s"), *FGuid::NewGuid().ToString())), TArray<uint8>());

	float MinPointTime = 0.f;
	uint8 PreviousHue = FMath::RandRange(0, 255);
	for (int32 i = 0; i < NumPoints; i++)
	{
		float NewPointTime = FMath::FRandRange(MinPointTime, (1.f / NumPoints) * i);
		
		// Generate a random hue value, optionally clamping how far it can deviate from the previous hue
		uint8 RandomHue = bClampColorDelta ? FMath::Clamp(PreviousHue + FMath::RandRange(-MaxDelta, MaxDelta), 0, 255) : FMath::RandRange(0, 255);
		FLinearColor NewPointColor = FLinearColor::MakeFromHSV8(RandomHue, 255, 255);
		
		// Add keys to each Red, Green, and Blue curve according to the random color that was generated
		for (int32 RGB = 0; RGB < 3; ++RGB)
		{
			float NewPointValue = 0.f;
			switch (RGB)
			{
				case 0: NewPointValue = NewPointColor.R; break;
				case 1: NewPointValue = NewPointColor.G; break;
				case 2: NewPointValue = NewPointColor.B; break;
			default: break;
			}
			NewGradient->FloatCurves[RGB].AddKey(NewPointTime, NewPointValue);
		}
		NewGradient->FloatCurves[3].AddKey(NewPointTime, 1.f); // All alpha values are forced to maximum
		
		MinPointTime = FMath::Min(NewPointTime + 0.1f, 1.f);
		PreviousHue = RandomHue;
	}

	return NewGradient;
}
