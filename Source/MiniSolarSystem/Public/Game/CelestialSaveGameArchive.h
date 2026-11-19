#pragma once
#include "Serialization\ObjectAndNameAsStringProxyArchive.h"
#include "Serialization\Archive.h"
#include "UObject/UObjectGlobals.h"

struct FCelestialSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
	FCelestialSaveGameArchive(FArchive& InInnerArchive)
		: FObjectAndNameAsStringProxyArchive(InInnerArchive, false)
	{
		ArIsSaveGame = true;
	}

	// Loading reads the object path, fixes up legacy paths, then resolves it like the base archive.
	// TObjectPtr properties are routed through this operator too (FArchiveUObject::SerializeObjectPtr)
	virtual FArchive& operator<<(UObject*& Obj) override
	{
		if (!IsLoading())
		{
			return FObjectAndNameAsStringProxyArchive::operator<<(Obj);
		}

		FString ObjectPath;
		InnerArchive << ObjectPath;
		if (ObjectPath.IsEmpty())
		{
			Obj = nullptr;
			return *this;
		}

		RemapLegacyObjectPath(ObjectPath);

		Obj = FindObject<UObject>(nullptr, *ObjectPath);
		if (!Obj && bLoadIfFindFails)
		{
			Obj = LoadObject<UObject>(nullptr, *ObjectPath);
		}
		return *this;
	}

	// Remaps object paths written by older versions of the game to where those objects live now
	static void RemapLegacyObjectPath(FString& ObjectPath)
	{
		// Runtime gradients are restored as "/Game/Materials/Gradients/Runtime/<Name>.<Name>" (CreateNewCurve), but older saves
		// reference them with a different package name:
		//   ".../Runtime/.<Name>"      all gradients shared one package named ".../Runtime/"
		//   ".../Runtime/None.<Name>"  gradient created with no name, auto-named inside a package called "None"
		// Any runtime gradient path whose package name differs from its object name is mapped to the restored location
		static const FString GradientDirectory = TEXT("/Game/Materials/Gradients/Runtime/");
		if (!ObjectPath.StartsWith(GradientDirectory))
		{
			return;
		}

		FString PackagePart;
		FString ObjectName;
		if (ObjectPath.RightChop(GradientDirectory.Len()).Split(TEXT("."), &PackagePart, &ObjectName) && PackagePart != ObjectName)
		{
			ObjectPath = GradientDirectory + ObjectName + TEXT(".") + ObjectName;
		}
	}
};
