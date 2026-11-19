// Copyright Soren Gilbertson

#include "Helpers/SaveDataBlueprintFunctionLibrary.h"
#include "UObject/Package.h"

UObject* UMSSAssetFunctionLibrary::FindLoadedAsset(const FString& PackageName, FName Name, UClass* Class)
{
	UPackage* Package = FindPackage(nullptr, *PackageName);
	if (!Package)
	{
		return nullptr;
	}

	UObject* Asset = StaticFindObjectFast(UObject::StaticClass(), Package, Name);
	return Asset && Asset->IsA(Class) ? Asset : nullptr;
}
