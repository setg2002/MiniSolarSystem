// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MiniSolarSystemTarget : TargetRules
{
	public MiniSolarSystemTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		CppStandard = CppStandardVersion.Latest;
		ExtraModuleNames.AddRange( new string[] { "MiniSolarSystem" } );
	}
}
