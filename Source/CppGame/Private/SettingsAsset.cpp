// Copyright Soren Gilbertson


#include "SettingsAsset.h"

USettingsAsset::USettingsAsset()
{
	if (Name == NAME_None)
		Name = FName(this->GetName());
}
