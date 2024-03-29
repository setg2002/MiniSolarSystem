// Copyright Soren Gilbertson

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	UFUNCTION(BlueprintCallable, Category = "Save")
	static bool SaveStruct(const StructClass& Struct, const FString& FilePath)
	{
		FString JsonString;
		FJsonObjectConverter::UStructToJsonObjectString(Struct, JsonString);

		return FFileHelper::SaveStringToFile(JsonString, *FilePath);
	};

	// Load a file to struct, returns true if loaded successfully
	template <typename StructClass>
	UFUNCTION(BlueprintCallable, Category = "Save")
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
	UFUNCTION(BlueprintCallable, Category = "Save")
	static bool SaveStruct_String(const StructClass& Struct)
	{
		FString JsonString;
		if (FJsonObjectConverter::UStructToJsonObjectString(Struct, JsonString))
		{
			return true;
		}
		return false;
	};

	// Load a Json string to struct
	template <typename StructClass>
	UFUNCTION(BlueprintCallable, Category = "Save")
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
