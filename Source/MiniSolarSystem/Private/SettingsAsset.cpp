// Copyright Soren Gilbertson


#include "SettingsAsset.h"


USettingsAsset::USettingsAsset()
{
	ID = this->GetUniqueID();
}

void USettingsAsset::AddAppliedID(uint32 NewID)
{
	IDs.AddUnique(NewID);
}

void USettingsAsset::RemoveAppliedID(uint32 IDToRemove)
{
	IDs.Remove(IDToRemove);
}

FName USettingsAsset::GetName()
{
	return this->GetFName();
}

bool USettingsAsset::SetName(FString NewName)
{
	return this->Rename(*NewName);
}

