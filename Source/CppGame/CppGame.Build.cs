// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CppGame : ModuleRules
{
	public CppGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ProceduralMeshComponent", "SimplexNoise", "RawMesh", "Niagara", "UMG", "Json", "JsonUtilities" });

        PrivateDependencyModuleNames.AddRange(new string[] { "LoadingScreen" });

        //MinFilesUsingPrecompiledHeaderOverride = 1;
        //bFasterWithoutUnity = true;

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
