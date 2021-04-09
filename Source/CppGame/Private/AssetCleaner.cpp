// This is a copyright notice


#include "AssetCleaner.h"
#include "FileHelpers.h"
#include "AssetQueryManager.h"
#include "ProjectCleanerUtility.h"
#include "UI/SProjectCleanerBrowser.h"


void AssetCleaner::CleanAll()
{
	static TArray<FString> StringDirectories = { "/Game/StaticMeshes", "/Game/Particles", "/Game/MaterialStuff/Gradients", "/Game/MaterialStuff/Instances", "/Game/Star", "/Game/Levels" };

	TArray<FDirectoryPath> Directories;

	for (int8 i = 0; i < StringDirectories.Num(); i++)
	{
		Directories.Add(FDirectoryPath());
		Directories[i].Path = StringDirectories[i];
	}

	Clean(Directories);
}

void AssetCleaner::CleanDirectory(EDirectoryFilterType Directory)
{
	static TArray<FString> StringDirectories = { "/Game/StaticMeshes", "/Game/Particles", "/Game/MaterialStuff/Gradients", "/Game/MaterialStuff/Instances", "/Game/Star", "/Game/Levels" };
	static TArray<FString> FilterDirectories = { "/Game/PlanetMeshes", "/Game/ProceduralTextures", "/Game/DataAssets" };

	TArray<FDirectoryPath> Directories;

	for (int8 i = 0; i < StringDirectories.Num(); i++)
	{
		Directories.Add(FDirectoryPath());
		Directories[i].Path = StringDirectories[i];
	}

	for (int8 i = 0; i < FilterDirectories.Num(); i++)
	{
		if (Directory != EDirectoryFilterType(i))
		{
			Directories.Add(FDirectoryPath());
			Directories[i].Path = FilterDirectories[i];
		}
	}

	Clean(Directories);
}


void AssetCleaner::CleanDirectories(TArray<EDirectoryFilterType> DirectoriesToClean)
{
	static TArray<FString> StringDirectories = { "/Game/StaticMeshes", "/Game/Particles", "/Game/MaterialStuff/Gradients", "/Game/MaterialStuff/Instances", "/Game/Star", "/Game/Levels" };
	static TArray<FString> FilterDirectories = { "/Game/PlanetMeshes", "/Game/ProceduralTextures", "/Game/DataAssets" };

	TArray<FDirectoryPath> Directories;

	for (int8 i = 0; i < StringDirectories.Num(); i++)
	{
		Directories.Add(FDirectoryPath());
		Directories[i].Path = StringDirectories[i];
	}

	for (int8 i = 0; i < DirectoriesToClean.Num(); i++)
	{
		if (!DirectoriesToClean.Contains(EDirectoryFilterType(i)))
		{
			Directories.Add(FDirectoryPath());
			Directories[i].Path = FilterDirectories[i];
		}

	}

	Clean(Directories);
}

void AssetCleaner::Clean(TArray<FDirectoryPath> Directories)
{
	FEditorFileUtils::SaveDirtyPackages(
		false,
		true,
		true
	);

	TArray<FAssetData> UnusedAssets;
	UDirectoryFilterSettings* DirectoryFilterSettings = GetMutableDefault<UDirectoryFilterSettings>();
	DirectoryFilterSettings->DirectoryPaths = Directories;
	TArray<FNode> AdjacencyList;

	AssetQueryManager::GetUnusedAssets(UnusedAssets, DirectoryFilterSettings, AdjacencyList);

	ProjectCleanerUtility::DeleteAssets(UnusedAssets);
	ProjectCleanerUtility::FixupRedirectors();
}
