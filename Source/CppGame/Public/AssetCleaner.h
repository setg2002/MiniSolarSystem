// This is a copyright notice

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

enum EDirectoryFilterType {
	Meshes,
	Textures,
	DataAssets
};


class CPPGAME_API AssetCleaner
{
public:
	static void CleanAll();

	static void CleanDirectory(EDirectoryFilterType Directory);

	static void CleanDirectories(TArray<EDirectoryFilterType> DirectoriesToClean);

private:
	static void Clean(TArray<FDirectoryPath> Directories);

};
