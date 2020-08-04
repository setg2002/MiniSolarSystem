// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SimplexNoise/Public/SimplexNoiseBPLibrary.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSimplexNoiseBPLibrary() {}
// Cross Module References
	SIMPLEXNOISE_API UScriptStruct* Z_Construct_UScriptStruct_FCell();
	UPackage* Z_Construct_UPackage__Script_SimplexNoise();
	SIMPLEXNOISE_API UScriptStruct* Z_Construct_UScriptStruct_FPoint();
	SIMPLEXNOISE_API UScriptStruct* Z_Construct_UScriptStruct_FTriangle();
	SIMPLEXNOISE_API UClass* Z_Construct_UClass_USimplexNoiseBPLibrary_NoRegister();
	SIMPLEXNOISE_API UClass* Z_Construct_UClass_USimplexNoiseBPLibrary();
	ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRandomStream();
// End Cross Module References
class UScriptStruct* FCell::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern SIMPLEXNOISE_API uint32 Get_Z_Construct_UScriptStruct_FCell_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FCell, Z_Construct_UPackage__Script_SimplexNoise(), TEXT("Cell"), sizeof(FCell), Get_Z_Construct_UScriptStruct_FCell_Hash());
	}
	return Singleton;
}
template<> SIMPLEXNOISE_API UScriptStruct* StaticStruct<FCell>()
{
	return FCell::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FCell(FCell::StaticStruct, TEXT("/Script/SimplexNoise"), TEXT("Cell"), false, nullptr, nullptr);
static struct FScriptStruct_SimplexNoise_StaticRegisterNativesFCell
{
	FScriptStruct_SimplexNoise_StaticRegisterNativesFCell()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("Cell")),new UScriptStruct::TCppStructOps<FCell>);
	}
} ScriptStruct_SimplexNoise_StaticRegisterNativesFCell;
	struct Z_Construct_UScriptStruct_FCell_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_point_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_point;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_val_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_val;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FCell_Statics::Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FCell_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FCell>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FCell_Statics::NewProp_point_MetaData[] = {
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FCell_Statics::NewProp_point = { "point", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, CPP_ARRAY_DIM(point, FCell), STRUCT_OFFSET(FCell, point), Z_Construct_UScriptStruct_FPoint, METADATA_PARAMS(Z_Construct_UScriptStruct_FCell_Statics::NewProp_point_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCell_Statics::NewProp_point_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FCell_Statics::NewProp_val_MetaData[] = {
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FCell_Statics::NewProp_val = { "val", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, CPP_ARRAY_DIM(val, FCell), STRUCT_OFFSET(FCell, val), METADATA_PARAMS(Z_Construct_UScriptStruct_FCell_Statics::NewProp_val_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCell_Statics::NewProp_val_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FCell_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCell_Statics::NewProp_point,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCell_Statics::NewProp_val,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FCell_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_SimplexNoise,
		nullptr,
		&NewStructOps,
		"Cell",
		sizeof(FCell),
		alignof(FCell),
		Z_Construct_UScriptStruct_FCell_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCell_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FCell_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCell_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FCell()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FCell_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_SimplexNoise();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("Cell"), sizeof(FCell), Get_Z_Construct_UScriptStruct_FCell_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FCell_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FCell_Hash() { return 1897841681U; }
class UScriptStruct* FTriangle::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern SIMPLEXNOISE_API uint32 Get_Z_Construct_UScriptStruct_FTriangle_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FTriangle, Z_Construct_UPackage__Script_SimplexNoise(), TEXT("Triangle"), sizeof(FTriangle), Get_Z_Construct_UScriptStruct_FTriangle_Hash());
	}
	return Singleton;
}
template<> SIMPLEXNOISE_API UScriptStruct* StaticStruct<FTriangle>()
{
	return FTriangle::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FTriangle(FTriangle::StaticStruct, TEXT("/Script/SimplexNoise"), TEXT("Triangle"), false, nullptr, nullptr);
static struct FScriptStruct_SimplexNoise_StaticRegisterNativesFTriangle
{
	FScriptStruct_SimplexNoise_StaticRegisterNativesFTriangle()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("Triangle")),new UScriptStruct::TCppStructOps<FTriangle>);
	}
} ScriptStruct_SimplexNoise_StaticRegisterNativesFTriangle;
	struct Z_Construct_UScriptStruct_FTriangle_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_point_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_point;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FTriangle_Statics::Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FTriangle_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FTriangle>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FTriangle_Statics::NewProp_point_MetaData[] = {
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FTriangle_Statics::NewProp_point = { "point", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, CPP_ARRAY_DIM(point, FTriangle), STRUCT_OFFSET(FTriangle, point), Z_Construct_UScriptStruct_FPoint, METADATA_PARAMS(Z_Construct_UScriptStruct_FTriangle_Statics::NewProp_point_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FTriangle_Statics::NewProp_point_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FTriangle_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FTriangle_Statics::NewProp_point,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FTriangle_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_SimplexNoise,
		nullptr,
		&NewStructOps,
		"Triangle",
		sizeof(FTriangle),
		alignof(FTriangle),
		Z_Construct_UScriptStruct_FTriangle_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FTriangle_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FTriangle_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FTriangle_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FTriangle()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FTriangle_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_SimplexNoise();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("Triangle"), sizeof(FTriangle), Get_Z_Construct_UScriptStruct_FTriangle_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FTriangle_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FTriangle_Hash() { return 1281451314U; }
class UScriptStruct* FPoint::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern SIMPLEXNOISE_API uint32 Get_Z_Construct_UScriptStruct_FPoint_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FPoint, Z_Construct_UPackage__Script_SimplexNoise(), TEXT("Point"), sizeof(FPoint), Get_Z_Construct_UScriptStruct_FPoint_Hash());
	}
	return Singleton;
}
template<> SIMPLEXNOISE_API UScriptStruct* StaticStruct<FPoint>()
{
	return FPoint::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FPoint(FPoint::StaticStruct, TEXT("/Script/SimplexNoise"), TEXT("Point"), false, nullptr, nullptr);
static struct FScriptStruct_SimplexNoise_StaticRegisterNativesFPoint
{
	FScriptStruct_SimplexNoise_StaticRegisterNativesFPoint()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("Point")),new UScriptStruct::TCppStructOps<FPoint>);
	}
} ScriptStruct_SimplexNoise_StaticRegisterNativesFPoint;
	struct Z_Construct_UScriptStruct_FPoint_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_x_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_y_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_z_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_z;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FPoint_Statics::Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FPoint_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FPoint>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FPoint_Statics::NewProp_x_MetaData[] = {
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FPoint_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FPoint, x), METADATA_PARAMS(Z_Construct_UScriptStruct_FPoint_Statics::NewProp_x_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPoint_Statics::NewProp_x_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FPoint_Statics::NewProp_y_MetaData[] = {
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FPoint_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FPoint, y), METADATA_PARAMS(Z_Construct_UScriptStruct_FPoint_Statics::NewProp_y_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPoint_Statics::NewProp_y_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FPoint_Statics::NewProp_z_MetaData[] = {
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FPoint_Statics::NewProp_z = { "z", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FPoint, z), METADATA_PARAMS(Z_Construct_UScriptStruct_FPoint_Statics::NewProp_z_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPoint_Statics::NewProp_z_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FPoint_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FPoint_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FPoint_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FPoint_Statics::NewProp_z,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FPoint_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_SimplexNoise,
		nullptr,
		&NewStructOps,
		"Point",
		sizeof(FPoint),
		alignof(FPoint),
		Z_Construct_UScriptStruct_FPoint_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPoint_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FPoint_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FPoint_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FPoint()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FPoint_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_SimplexNoise();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("Point"), sizeof(FPoint), Get_Z_Construct_UScriptStruct_FPoint_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FPoint_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FPoint_Hash() { return 369178749U; }
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execGetSimplexNoise4D_EX)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_y);
		P_GET_PROPERTY(FFloatProperty,Z_Param_z);
		P_GET_PROPERTY(FFloatProperty,Z_Param_w);
		P_GET_PROPERTY(FFloatProperty,Z_Param_lacunarity);
		P_GET_PROPERTY(FFloatProperty,Z_Param_persistance);
		P_GET_PROPERTY(FIntProperty,Z_Param_octaves);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_GET_UBOOL(Z_Param_ZeroToOne);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::GetSimplexNoise4D_EX(Z_Param_x,Z_Param_y,Z_Param_z,Z_Param_w,Z_Param_lacunarity,Z_Param_persistance,Z_Param_octaves,Z_Param_inFactor,Z_Param_ZeroToOne);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execGetSimplexNoise3D_EX)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_y);
		P_GET_PROPERTY(FFloatProperty,Z_Param_z);
		P_GET_PROPERTY(FFloatProperty,Z_Param_lacunarity);
		P_GET_PROPERTY(FFloatProperty,Z_Param_persistance);
		P_GET_PROPERTY(FIntProperty,Z_Param_octaves);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_GET_UBOOL(Z_Param_ZeroToOne);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::GetSimplexNoise3D_EX(Z_Param_x,Z_Param_y,Z_Param_z,Z_Param_lacunarity,Z_Param_persistance,Z_Param_octaves,Z_Param_inFactor,Z_Param_ZeroToOne);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execGetSimplexNoise2D_EX)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_y);
		P_GET_PROPERTY(FFloatProperty,Z_Param_lacunarity);
		P_GET_PROPERTY(FFloatProperty,Z_Param_persistance);
		P_GET_PROPERTY(FIntProperty,Z_Param_octaves);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_GET_UBOOL(Z_Param_ZeroToOne);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(Z_Param_x,Z_Param_y,Z_Param_lacunarity,Z_Param_persistance,Z_Param_octaves,Z_Param_inFactor,Z_Param_ZeroToOne);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execGetSimplexNoise1D_EX)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_lacunarity);
		P_GET_PROPERTY(FFloatProperty,Z_Param_persistance);
		P_GET_PROPERTY(FIntProperty,Z_Param_octaves);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_GET_UBOOL(Z_Param_ZeroToOne);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::GetSimplexNoise1D_EX(Z_Param_x,Z_Param_lacunarity,Z_Param_persistance,Z_Param_octaves,Z_Param_inFactor,Z_Param_ZeroToOne);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execSimplexNoiseInRange4D)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_y);
		P_GET_PROPERTY(FFloatProperty,Z_Param_z);
		P_GET_PROPERTY(FFloatProperty,Z_Param_w);
		P_GET_PROPERTY(FFloatProperty,Z_Param_rangeMin);
		P_GET_PROPERTY(FFloatProperty,Z_Param_rangeMax);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::SimplexNoiseInRange4D(Z_Param_x,Z_Param_y,Z_Param_z,Z_Param_w,Z_Param_rangeMin,Z_Param_rangeMax,Z_Param_inFactor);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execSimplexNoiseInRange3D)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_y);
		P_GET_PROPERTY(FFloatProperty,Z_Param_z);
		P_GET_PROPERTY(FFloatProperty,Z_Param_rangeMin);
		P_GET_PROPERTY(FFloatProperty,Z_Param_rangeMax);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::SimplexNoiseInRange3D(Z_Param_x,Z_Param_y,Z_Param_z,Z_Param_rangeMin,Z_Param_rangeMax,Z_Param_inFactor);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execSimplexNoiseInRange2D)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_y);
		P_GET_PROPERTY(FFloatProperty,Z_Param_rangeMin);
		P_GET_PROPERTY(FFloatProperty,Z_Param_rangeMax);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::SimplexNoiseInRange2D(Z_Param_x,Z_Param_y,Z_Param_rangeMin,Z_Param_rangeMax,Z_Param_inFactor);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execSimplexNoiseInRange1D)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_rangeMin);
		P_GET_PROPERTY(FFloatProperty,Z_Param_rangeMax);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::SimplexNoiseInRange1D(Z_Param_x,Z_Param_rangeMin,Z_Param_rangeMax,Z_Param_inFactor);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execSimplexNoiseScaled4D)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_y);
		P_GET_PROPERTY(FFloatProperty,Z_Param_z);
		P_GET_PROPERTY(FFloatProperty,Z_Param_w);
		P_GET_PROPERTY(FFloatProperty,Z_Param_scaleOut);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::SimplexNoiseScaled4D(Z_Param_x,Z_Param_y,Z_Param_z,Z_Param_w,Z_Param_scaleOut,Z_Param_inFactor);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execSimplexNoiseScaled3D)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_y);
		P_GET_PROPERTY(FFloatProperty,Z_Param_z);
		P_GET_PROPERTY(FFloatProperty,Z_Param_scaleOut);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::SimplexNoiseScaled3D(Z_Param_x,Z_Param_y,Z_Param_z,Z_Param_scaleOut,Z_Param_inFactor);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execSimplexNoiseScaled2D)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_y);
		P_GET_PROPERTY(FFloatProperty,Z_Param_scaleOut);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::SimplexNoiseScaled2D(Z_Param_x,Z_Param_y,Z_Param_scaleOut,Z_Param_inFactor);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execSimplexNoiseScaled1D)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_scaleOut);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::SimplexNoiseScaled1D(Z_Param_x,Z_Param_scaleOut,Z_Param_inFactor);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execSimplexNoise4D)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_y);
		P_GET_PROPERTY(FFloatProperty,Z_Param_z);
		P_GET_PROPERTY(FFloatProperty,Z_Param_w);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::SimplexNoise4D(Z_Param_x,Z_Param_y,Z_Param_z,Z_Param_w,Z_Param_inFactor);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execSimplexNoise3D)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_y);
		P_GET_PROPERTY(FFloatProperty,Z_Param_z);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::SimplexNoise3D(Z_Param_x,Z_Param_y,Z_Param_z,Z_Param_inFactor);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execSimplexNoise2D)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_y);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::SimplexNoise2D(Z_Param_x,Z_Param_y,Z_Param_inFactor);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execSimplexNoise1D)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_x);
		P_GET_PROPERTY(FFloatProperty,Z_Param_inFactor);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=USimplexNoiseBPLibrary::SimplexNoise1D(Z_Param_x,Z_Param_inFactor);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execsetNoiseFromStream)
	{
		P_GET_STRUCT_REF(FRandomStream,Z_Param_Out_RandStream);
		P_FINISH;
		P_NATIVE_BEGIN;
		USimplexNoiseBPLibrary::setNoiseFromStream(Z_Param_Out_RandStream);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USimplexNoiseBPLibrary::execsetNoiseSeed)
	{
		P_GET_PROPERTY_REF(FIntProperty,Z_Param_Out_newSeed);
		P_FINISH;
		P_NATIVE_BEGIN;
		USimplexNoiseBPLibrary::setNoiseSeed(Z_Param_Out_newSeed);
		P_NATIVE_END;
	}
	void USimplexNoiseBPLibrary::StaticRegisterNativesUSimplexNoiseBPLibrary()
	{
		UClass* Class = USimplexNoiseBPLibrary::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetSimplexNoise1D_EX", &USimplexNoiseBPLibrary::execGetSimplexNoise1D_EX },
			{ "GetSimplexNoise2D_EX", &USimplexNoiseBPLibrary::execGetSimplexNoise2D_EX },
			{ "GetSimplexNoise3D_EX", &USimplexNoiseBPLibrary::execGetSimplexNoise3D_EX },
			{ "GetSimplexNoise4D_EX", &USimplexNoiseBPLibrary::execGetSimplexNoise4D_EX },
			{ "setNoiseFromStream", &USimplexNoiseBPLibrary::execsetNoiseFromStream },
			{ "setNoiseSeed", &USimplexNoiseBPLibrary::execsetNoiseSeed },
			{ "SimplexNoise1D", &USimplexNoiseBPLibrary::execSimplexNoise1D },
			{ "SimplexNoise2D", &USimplexNoiseBPLibrary::execSimplexNoise2D },
			{ "SimplexNoise3D", &USimplexNoiseBPLibrary::execSimplexNoise3D },
			{ "SimplexNoise4D", &USimplexNoiseBPLibrary::execSimplexNoise4D },
			{ "SimplexNoiseInRange1D", &USimplexNoiseBPLibrary::execSimplexNoiseInRange1D },
			{ "SimplexNoiseInRange2D", &USimplexNoiseBPLibrary::execSimplexNoiseInRange2D },
			{ "SimplexNoiseInRange3D", &USimplexNoiseBPLibrary::execSimplexNoiseInRange3D },
			{ "SimplexNoiseInRange4D", &USimplexNoiseBPLibrary::execSimplexNoiseInRange4D },
			{ "SimplexNoiseScaled1D", &USimplexNoiseBPLibrary::execSimplexNoiseScaled1D },
			{ "SimplexNoiseScaled2D", &USimplexNoiseBPLibrary::execSimplexNoiseScaled2D },
			{ "SimplexNoiseScaled3D", &USimplexNoiseBPLibrary::execSimplexNoiseScaled3D },
			{ "SimplexNoiseScaled4D", &USimplexNoiseBPLibrary::execSimplexNoiseScaled4D },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics
	{
		struct SimplexNoiseBPLibrary_eventGetSimplexNoise1D_EX_Parms
		{
			float x;
			float lacunarity;
			float persistance;
			int32 octaves;
			float inFactor;
			bool ZeroToOne;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_lacunarity;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_persistance;
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_octaves;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static void NewProp_ZeroToOne_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ZeroToOne;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise1D_EX_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_lacunarity = { "lacunarity", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise1D_EX_Parms, lacunarity), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_persistance = { "persistance", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise1D_EX_Parms, persistance), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_octaves = { "octaves", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise1D_EX_Parms, octaves), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise1D_EX_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_ZeroToOne_SetBit(void* Obj)
	{
		((SimplexNoiseBPLibrary_eventGetSimplexNoise1D_EX_Parms*)Obj)->ZeroToOne = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_ZeroToOne = { "ZeroToOne", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(SimplexNoiseBPLibrary_eventGetSimplexNoise1D_EX_Parms), &Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_ZeroToOne_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise1D_EX_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_lacunarity,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_persistance,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_octaves,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_ZeroToOne,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "CPP_Default_lacunarity", "2.300000" },
		{ "CPP_Default_octaves", "4" },
		{ "CPP_Default_persistance", "0.600000" },
		{ "CPP_Default_ZeroToOne", "false" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "GetSimplexNoise1D_EX", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventGetSimplexNoise1D_EX_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics
	{
		struct SimplexNoiseBPLibrary_eventGetSimplexNoise2D_EX_Parms
		{
			float x;
			float y;
			float lacunarity;
			float persistance;
			int32 octaves;
			float inFactor;
			bool ZeroToOne;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_lacunarity;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_persistance;
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_octaves;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static void NewProp_ZeroToOne_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ZeroToOne;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise2D_EX_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise2D_EX_Parms, y), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_lacunarity = { "lacunarity", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise2D_EX_Parms, lacunarity), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_persistance = { "persistance", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise2D_EX_Parms, persistance), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_octaves = { "octaves", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise2D_EX_Parms, octaves), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise2D_EX_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_ZeroToOne_SetBit(void* Obj)
	{
		((SimplexNoiseBPLibrary_eventGetSimplexNoise2D_EX_Parms*)Obj)->ZeroToOne = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_ZeroToOne = { "ZeroToOne", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(SimplexNoiseBPLibrary_eventGetSimplexNoise2D_EX_Parms), &Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_ZeroToOne_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise2D_EX_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_lacunarity,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_persistance,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_octaves,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_ZeroToOne,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "CPP_Default_lacunarity", "2.300000" },
		{ "CPP_Default_octaves", "4" },
		{ "CPP_Default_persistance", "0.600000" },
		{ "CPP_Default_ZeroToOne", "false" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "GetSimplexNoise2D_EX", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventGetSimplexNoise2D_EX_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics
	{
		struct SimplexNoiseBPLibrary_eventGetSimplexNoise3D_EX_Parms
		{
			float x;
			float y;
			float z;
			float lacunarity;
			float persistance;
			int32 octaves;
			float inFactor;
			bool ZeroToOne;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_z;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_lacunarity;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_persistance;
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_octaves;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static void NewProp_ZeroToOne_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ZeroToOne;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise3D_EX_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise3D_EX_Parms, y), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_z = { "z", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise3D_EX_Parms, z), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_lacunarity = { "lacunarity", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise3D_EX_Parms, lacunarity), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_persistance = { "persistance", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise3D_EX_Parms, persistance), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_octaves = { "octaves", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise3D_EX_Parms, octaves), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise3D_EX_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_ZeroToOne_SetBit(void* Obj)
	{
		((SimplexNoiseBPLibrary_eventGetSimplexNoise3D_EX_Parms*)Obj)->ZeroToOne = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_ZeroToOne = { "ZeroToOne", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(SimplexNoiseBPLibrary_eventGetSimplexNoise3D_EX_Parms), &Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_ZeroToOne_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise3D_EX_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_z,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_lacunarity,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_persistance,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_octaves,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_ZeroToOne,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "CPP_Default_lacunarity", "2.300000" },
		{ "CPP_Default_octaves", "4" },
		{ "CPP_Default_persistance", "0.600000" },
		{ "CPP_Default_ZeroToOne", "false" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "GetSimplexNoise3D_EX", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventGetSimplexNoise3D_EX_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics
	{
		struct SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms
		{
			float x;
			float y;
			float z;
			float w;
			float lacunarity;
			float persistance;
			int32 octaves;
			float inFactor;
			bool ZeroToOne;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_z;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_w;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_lacunarity;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_persistance;
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_octaves;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static void NewProp_ZeroToOne_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ZeroToOne;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms, y), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_z = { "z", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms, z), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_w = { "w", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms, w), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_lacunarity = { "lacunarity", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms, lacunarity), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_persistance = { "persistance", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms, persistance), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_octaves = { "octaves", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms, octaves), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_ZeroToOne_SetBit(void* Obj)
	{
		((SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms*)Obj)->ZeroToOne = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_ZeroToOne = { "ZeroToOne", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms), &Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_ZeroToOne_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_z,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_w,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_lacunarity,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_persistance,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_octaves,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_ZeroToOne,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "CPP_Default_lacunarity", "2.300000" },
		{ "CPP_Default_octaves", "4" },
		{ "CPP_Default_persistance", "0.600000" },
		{ "CPP_Default_ZeroToOne", "false" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "GetSimplexNoise4D_EX", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventGetSimplexNoise4D_EX_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream_Statics
	{
		struct SimplexNoiseBPLibrary_eventsetNoiseFromStream_Parms
		{
			FRandomStream RandStream;
		};
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_RandStream;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream_Statics::NewProp_RandStream = { "RandStream", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventsetNoiseFromStream_Parms, RandStream), Z_Construct_UScriptStruct_FRandomStream, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream_Statics::NewProp_RandStream,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "setNoiseFromStream", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventsetNoiseFromStream_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C22401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics
	{
		struct SimplexNoiseBPLibrary_eventsetNoiseSeed_Parms
		{
			int32 newSeed;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_newSeed_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_newSeed;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::NewProp_newSeed_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::NewProp_newSeed = { "newSeed", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventsetNoiseSeed_Parms, newSeed), METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::NewProp_newSeed_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::NewProp_newSeed_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::NewProp_newSeed,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "setNoiseSeed", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventsetNoiseSeed_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics
	{
		struct SimplexNoiseBPLibrary_eventSimplexNoise1D_Parms
		{
			float x;
			float inFactor;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise1D_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise1D_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise1D_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "SimplexNoise1D", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventSimplexNoise1D_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics
	{
		struct SimplexNoiseBPLibrary_eventSimplexNoise2D_Parms
		{
			float x;
			float y;
			float inFactor;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise2D_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise2D_Parms, y), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise2D_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise2D_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "SimplexNoise2D", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventSimplexNoise2D_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics
	{
		struct SimplexNoiseBPLibrary_eventSimplexNoise3D_Parms
		{
			float x;
			float y;
			float z;
			float inFactor;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_z;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise3D_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise3D_Parms, y), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::NewProp_z = { "z", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise3D_Parms, z), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise3D_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise3D_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::NewProp_z,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "SimplexNoise3D", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventSimplexNoise3D_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics
	{
		struct SimplexNoiseBPLibrary_eventSimplexNoise4D_Parms
		{
			float x;
			float y;
			float z;
			float w;
			float inFactor;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_z;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_w;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise4D_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise4D_Parms, y), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::NewProp_z = { "z", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise4D_Parms, z), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::NewProp_w = { "w", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise4D_Parms, w), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise4D_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoise4D_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::NewProp_z,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::NewProp_w,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "SimplexNoise4D", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventSimplexNoise4D_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics
	{
		struct SimplexNoiseBPLibrary_eventSimplexNoiseInRange1D_Parms
		{
			float x;
			float rangeMin;
			float rangeMax;
			float inFactor;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_rangeMin;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_rangeMax;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange1D_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::NewProp_rangeMin = { "rangeMin", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange1D_Parms, rangeMin), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::NewProp_rangeMax = { "rangeMax", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange1D_Parms, rangeMax), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange1D_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange1D_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::NewProp_rangeMin,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::NewProp_rangeMax,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "Comment", "// Return value in Range between two float numbers\n// Return Value is scaled by difference between rangeMin & rangeMax value\n" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
		{ "ToolTip", "Return value in Range between two float numbers\nReturn Value is scaled by difference between rangeMin & rangeMax value" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "SimplexNoiseInRange1D", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventSimplexNoiseInRange1D_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics
	{
		struct SimplexNoiseBPLibrary_eventSimplexNoiseInRange2D_Parms
		{
			float x;
			float y;
			float rangeMin;
			float rangeMax;
			float inFactor;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_rangeMin;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_rangeMax;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange2D_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange2D_Parms, y), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::NewProp_rangeMin = { "rangeMin", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange2D_Parms, rangeMin), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::NewProp_rangeMax = { "rangeMax", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange2D_Parms, rangeMax), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange2D_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange2D_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::NewProp_rangeMin,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::NewProp_rangeMax,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "SimplexNoiseInRange2D", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventSimplexNoiseInRange2D_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics
	{
		struct SimplexNoiseBPLibrary_eventSimplexNoiseInRange3D_Parms
		{
			float x;
			float y;
			float z;
			float rangeMin;
			float rangeMax;
			float inFactor;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_z;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_rangeMin;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_rangeMax;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange3D_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange3D_Parms, y), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_z = { "z", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange3D_Parms, z), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_rangeMin = { "rangeMin", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange3D_Parms, rangeMin), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_rangeMax = { "rangeMax", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange3D_Parms, rangeMax), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange3D_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange3D_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_z,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_rangeMin,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_rangeMax,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "SimplexNoiseInRange3D", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventSimplexNoiseInRange3D_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics
	{
		struct SimplexNoiseBPLibrary_eventSimplexNoiseInRange4D_Parms
		{
			float x;
			float y;
			float z;
			float w;
			float rangeMin;
			float rangeMax;
			float inFactor;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_z;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_w;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_rangeMin;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_rangeMax;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange4D_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange4D_Parms, y), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_z = { "z", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange4D_Parms, z), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_w = { "w", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange4D_Parms, w), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_rangeMin = { "rangeMin", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange4D_Parms, rangeMin), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_rangeMax = { "rangeMax", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange4D_Parms, rangeMax), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange4D_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseInRange4D_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_z,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_w,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_rangeMin,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_rangeMax,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "SimplexNoiseInRange4D", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventSimplexNoiseInRange4D_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics
	{
		struct SimplexNoiseBPLibrary_eventSimplexNoiseScaled1D_Parms
		{
			float x;
			float scaleOut;
			float inFactor;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_scaleOut;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled1D_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::NewProp_scaleOut = { "scaleOut", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled1D_Parms, scaleOut), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled1D_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled1D_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::NewProp_scaleOut,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "Comment", "// Scaled by float value\n" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "CPP_Default_scaleOut", "1.000000" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
		{ "ToolTip", "Scaled by float value" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "SimplexNoiseScaled1D", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventSimplexNoiseScaled1D_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics
	{
		struct SimplexNoiseBPLibrary_eventSimplexNoiseScaled2D_Parms
		{
			float x;
			float y;
			float scaleOut;
			float inFactor;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_scaleOut;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled2D_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled2D_Parms, y), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::NewProp_scaleOut = { "scaleOut", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled2D_Parms, scaleOut), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled2D_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled2D_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::NewProp_scaleOut,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "CPP_Default_scaleOut", "1.000000" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "SimplexNoiseScaled2D", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventSimplexNoiseScaled2D_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics
	{
		struct SimplexNoiseBPLibrary_eventSimplexNoiseScaled3D_Parms
		{
			float x;
			float y;
			float z;
			float scaleOut;
			float inFactor;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_z;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_scaleOut;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled3D_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled3D_Parms, y), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::NewProp_z = { "z", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled3D_Parms, z), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::NewProp_scaleOut = { "scaleOut", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled3D_Parms, scaleOut), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled3D_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled3D_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::NewProp_z,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::NewProp_scaleOut,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "CPP_Default_scaleOut", "1.000000" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "SimplexNoiseScaled3D", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventSimplexNoiseScaled3D_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics
	{
		struct SimplexNoiseBPLibrary_eventSimplexNoiseScaled4D_Parms
		{
			float x;
			float y;
			float z;
			float w;
			float scaleOut;
			float inFactor;
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_x;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_y;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_z;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_w;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_scaleOut;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_inFactor;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_x = { "x", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled4D_Parms, x), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_y = { "y", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled4D_Parms, y), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_z = { "z", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled4D_Parms, z), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_w = { "w", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled4D_Parms, w), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_scaleOut = { "scaleOut", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled4D_Parms, scaleOut), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_inFactor = { "inFactor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled4D_Parms, inFactor), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SimplexNoiseBPLibrary_eventSimplexNoiseScaled4D_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_x,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_y,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_z,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_w,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_scaleOut,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_inFactor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::Function_MetaDataParams[] = {
		{ "Category", "SimplexNoise" },
		{ "CPP_Default_inFactor", "1.000000" },
		{ "CPP_Default_scaleOut", "1.000000" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USimplexNoiseBPLibrary, nullptr, "SimplexNoiseScaled4D", nullptr, nullptr, sizeof(SimplexNoiseBPLibrary_eventSimplexNoiseScaled4D_Parms), Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_USimplexNoiseBPLibrary_NoRegister()
	{
		return USimplexNoiseBPLibrary::StaticClass();
	}
	struct Z_Construct_UClass_USimplexNoiseBPLibrary_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_USimplexNoiseBPLibrary_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
		(UObject* (*)())Z_Construct_UPackage__Script_SimplexNoise,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_USimplexNoiseBPLibrary_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise1D_EX, "GetSimplexNoise1D_EX" }, // 118807461
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise2D_EX, "GetSimplexNoise2D_EX" }, // 1777734683
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise3D_EX, "GetSimplexNoise3D_EX" }, // 1945794694
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_GetSimplexNoise4D_EX, "GetSimplexNoise4D_EX" }, // 562044418
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseFromStream, "setNoiseFromStream" }, // 4160295737
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_setNoiseSeed, "setNoiseSeed" }, // 572515926
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise1D, "SimplexNoise1D" }, // 3041074933
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise2D, "SimplexNoise2D" }, // 2089922473
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise3D, "SimplexNoise3D" }, // 3982184552
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoise4D, "SimplexNoise4D" }, // 3312581383
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange1D, "SimplexNoiseInRange1D" }, // 2060898687
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange2D, "SimplexNoiseInRange2D" }, // 1487378688
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange3D, "SimplexNoiseInRange3D" }, // 4106355248
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseInRange4D, "SimplexNoiseInRange4D" }, // 3993394879
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled1D, "SimplexNoiseScaled1D" }, // 1572259477
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled2D, "SimplexNoiseScaled2D" }, // 980392624
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled3D, "SimplexNoiseScaled3D" }, // 3906893924
		{ &Z_Construct_UFunction_USimplexNoiseBPLibrary_SimplexNoiseScaled4D, "SimplexNoiseScaled4D" }, // 2272042921
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USimplexNoiseBPLibrary_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "SimplexNoiseBPLibrary.h" },
		{ "ModuleRelativePath", "Public/SimplexNoiseBPLibrary.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_USimplexNoiseBPLibrary_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<USimplexNoiseBPLibrary>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_USimplexNoiseBPLibrary_Statics::ClassParams = {
		&USimplexNoiseBPLibrary::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_USimplexNoiseBPLibrary_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_USimplexNoiseBPLibrary_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_USimplexNoiseBPLibrary()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_USimplexNoiseBPLibrary_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(USimplexNoiseBPLibrary, 3777496253);
	template<> SIMPLEXNOISE_API UClass* StaticClass<USimplexNoiseBPLibrary>()
	{
		return USimplexNoiseBPLibrary::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_USimplexNoiseBPLibrary(Z_Construct_UClass_USimplexNoiseBPLibrary, &USimplexNoiseBPLibrary::StaticClass, TEXT("/Script/SimplexNoise"), TEXT("USimplexNoiseBPLibrary"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(USimplexNoiseBPLibrary);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
