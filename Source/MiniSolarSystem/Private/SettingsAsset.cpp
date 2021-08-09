// Copyright Soren Gilbertson


#include "SettingsAsset.h"

USettingsAsset::USettingsAsset()
{

}

FName USettingsAsset::GetName()
{
	return this->GetFName();
}

bool USettingsAsset::SetName(FString NewName)
{
	return this->Rename(*NewName);
}

