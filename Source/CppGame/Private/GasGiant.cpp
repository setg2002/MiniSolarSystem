// This is a copyright notice


#include "GasGiant.h"
#include "GasGiantColorSettings.h"
#include "AssetRegistryModule.h"
#include "Curves/CurveLinearColor.h"


AGasGiant::AGasGiant()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
}

void AGasGiant::GenerateMaterial()
{
	CreateTexture("GasTexture", ColorSettings->Gradient);
	ensure(ColorSettings->BasePlanetMat);
	ColorSettings->DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, ColorSettings->BasePlanetMat);
}

UTexture2D* AGasGiant::CreateTexture(FString TextureName, UCurveLinearColor* Gradient)
{
	FString PackageName = TEXT("/Game/ProceduralTextures/" + this->GetName() + "_" + TextureName);
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
	FTexture2DMipMap* Mip = new(NewTexture->PlatformData->Mips) FTexture2DMipMap();
	Mip->SizeX = 256;
	Mip->SizeY = NewTexture->PlatformData->SizeY;

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

void AGasGiant::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName(PropertyChangedEvent.Property->GetName());

		if (PropertyName == GET_MEMBER_NAME_CHECKED(UGasGiantColorSettings, BasePlanetMat))
		{
			ColorSettings->DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, ColorSettings->BasePlanetMat);
		}
	}
}
