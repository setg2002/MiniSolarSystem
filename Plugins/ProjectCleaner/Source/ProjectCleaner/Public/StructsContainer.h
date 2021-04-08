﻿#pragma once

#include "CoreMinimal.h"

struct FCleaningStats
{
	int32 UnusedAssetsNum;
	int32 EmptyFolders;
	int64 UnusedAssetsTotalSize;
	int32 DeletedAssetCount;
	int32 TotalAssetNum;

	FCleaningStats()
	{
		UnusedAssetsNum = 0;
		EmptyFolders = 0;
		UnusedAssetsTotalSize = 0;
		DeletedAssetCount = 0;
		TotalAssetNum = 0;
	}

	int32 GetPercentage() const
	{
		return (DeletedAssetCount * 100.0f) / TotalAssetNum;
	}
};

struct FStandardCleanerText
{
	FText AssetsDeleteWindowTitle;
	FText AssetsDeleteWindowContent;
	FText EmptyFolderWindowTitle;
	FText EmptyFolderWindowContent;
	FText StartingCleanup;
	FText NoAssetsToDelete;
	FText NoEmptyFolderToDelete;
	FText NonUAssetFilesFound;
	FText SearchingEmptyFolders;

	FStandardCleanerText()
	{
		AssetsDeleteWindowTitle = FText::FromString("Confirm deletion");
		AssetsDeleteWindowContent = FText::FromString("Are you sure you want to permanently delete unused assets?");
		EmptyFolderWindowTitle = FText::FromString("Confirm deletion of empty folders");
		EmptyFolderWindowContent = FText::FromString("Are you sure you want to delete all empty folders in project?");
		StartingCleanup = FText::FromString("Starting Cleanup. This could take some time, please wait");
		NoAssetsToDelete = FText::FromString("There are no assets to delete!");
		NoEmptyFolderToDelete = FText::FromString("There are no empty folders to delete!");
		NonUAssetFilesFound = FText::FromString("Project contains non .uasset files. Check Output Log for more info.");
		SearchingEmptyFolders = FText::FromString("Searching empty folders...");
	}
};


/**
 * @brief Adjacency List Node
 */
struct FNode
{
	FName Asset;

	TArray<FName> AdjacentAssets;
};
