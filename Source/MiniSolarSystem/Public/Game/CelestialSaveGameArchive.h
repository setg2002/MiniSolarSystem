#pragma once
#include "Serialization\ObjectAndNameAsStringProxyArchive.h"
#include "Serialization\Archive.h"

struct FCelestialSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
	FCelestialSaveGameArchive(FArchive& InInnerArchive)
		: FObjectAndNameAsStringProxyArchive(InInnerArchive, false)
	{
		ArIsSaveGame = true;
	}
};