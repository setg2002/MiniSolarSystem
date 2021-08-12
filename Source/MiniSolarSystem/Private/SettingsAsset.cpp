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

FName USettingsAsset::GetName()
{
	return this->GetFName();
}

bool USettingsAsset::SetName(FString NewName)
{
	return this->Rename(*NewName);
}

