// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ISettingsAssetID.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "SaveDataBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MINISOLARSYSTEM_API USaveDataBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Saves a struct to file, returns true if saved successfully
	template <typename StructClass>
	static bool SaveStruct(const StructClass& Struct, const FString& FilePath)
	{
		FString JsonString;
		FJsonObjectConverter::UStructToJsonObjectString(Struct, JsonString);

		return FFileHelper::SaveStringToFile(JsonString, *FilePath);
	};

	// Load a file to struct, returns true if loaded successfully
	template <typename StructClass>
	static bool LoadStruct(const FString& FilePath, StructClass& OutStruct)
	{
		if (!IFileManager::Get().FileExists(*FilePath))
		{
			return false;
		}

		FString JsonString;
		if (FFileHelper::LoadFileToString(JsonString, *FilePath))
		{
			FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &OutStruct, 0, 0);
			return true;
		}

		return false;
	}

	// Converts struct to FString
	template <typename StructClass>
	static bool SaveStruct_String(const StructClass& Struct)
	{
		FString JsonString;
		if (FJsonObjectConverter::UStructToJsonObjectString(Struct, JsonString))
		{
			return true;
		}
		return false;
	}

	// Load a Json string to struct
	template <typename StructClass>
	static bool LoadStruct_String(const FString& String, StructClass& OutStruct)
	{
		FString JsonString;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &OutStruct, 0, 0))
		{
			return true;
		}
		return false;
	}
	
};


UCLASS()
class MINISOLARSYSTEM_API UMSSAssetFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	template <class T>
	static T* FindSettingsAssetFromAppliedID(uint32 ID)
	{
		UClass* Class = T::StaticClass();
		if (!IsValid(Class))
		{
			return nullptr;
		}
		
		if (!Class->ImplementsInterface(USettingsAssetID::StaticClass()))
		{
			return nullptr;
		}
		
		TArray<FAssetData> AssetData;
		IAssetRegistry::Get()->GetAssetsByClass(Class->GetClassPathName(), AssetData);
		
		for (const FAssetData& Asset : AssetData)
		{
			UObject* LoadedAsset = Asset.GetAsset();
			if (!IsValid(LoadedAsset))
			{
				continue;
			}
			
			ISettingsAssetID* LoadedSettingsAsset = Cast<ISettingsAssetID>(LoadedAsset);
			if (!LoadedSettingsAsset || !LoadedSettingsAsset->GetAppliedIDs().Contains(ID))
			{
				continue;
			}

			if (T* LoadedTypedAsset = Cast<T>(LoadedAsset))
			{
				return LoadedTypedAsset;
			}
		}
			
		return nullptr;
	}
	
	template <class T>
	static TArray<T*> FindSettingsAssetsFromAppliedID(uint32 ID)
	{
		UClass* Class = T::StaticClass();
		if (!IsValid(Class))
		{
			return TArray<T*>();
		}
		
		if (!Class->ImplementsInterface(USettingsAssetID::StaticClass()))
		{
			return TArray<T*>();
		}
		
		TArray<FAssetData> AssetData;
		IAssetRegistry::Get()->GetAssetsByClass(Class->GetClassPathName(), AssetData);
		
		TArray<T*> OutAssets;
		for (const FAssetData& Asset : AssetData)
		{
			UObject* LoadedAsset = Asset.GetAsset();
			if (!IsValid(LoadedAsset))
			{
				continue;
			}
			
			ISettingsAssetID* LoadedSettingsAsset = Cast<ISettingsAssetID>(LoadedAsset);
			if (!LoadedSettingsAsset || !LoadedSettingsAsset->GetAppliedIDs().Contains(ID))
			{
				continue;
			}

			if (T* LoadedTypedAsset = Cast<T>(LoadedAsset))
			{
				OutAssets.Add(LoadedTypedAsset);
			}
		}
			
		return OutAssets;
	}
	
};
