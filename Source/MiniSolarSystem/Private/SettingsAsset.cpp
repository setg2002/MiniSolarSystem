// Copyright Soren Gilbertson


#include "SettingsAsset.h"


USettingsAsset::USettingsAsset()
{
	ID = this->GetUniqueID();
	if (LocalName.IsEmpty())
		Cast<UObject>(this)->GetName(LocalName);
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
	return (FName)LocalName;//this->GetFName();
}

bool USettingsAsset::SetName(FString NewName)
{
	//FString OldName = Cast<UObject>(this)->GetName();
	//bool b = this->Rename(*NewName);
	//if (b)
		LocalName = NewName;
	//this->Rename(*OldName);
	return true;//b;
}

