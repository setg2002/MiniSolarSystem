// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using System.IO;


public class CppGameEditorTarget : TargetRules
{
	public CppGameEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

        // Absolute path to the GameVersion header file
        const string GAME_VERSION_FILE              = @"G:\UnrealProjects\CppGame\Source\CppGame\Public\GameVersion.h";

        // Absolute path to the GAME_BUILD text file
        const string GAME_BUILD_FILE                = @"G:\UnrealProjects\CppGame\Source\CppGame\GAME_BUILD.txt";

        // Text to replace in GameVersion header file
        const string BUILD_NUMBER_TEXT              = "#define GAME_BUILD_NUMBER";

        // Line number to replace. Should be the real line number minus 1. That is if #define GAME_BUILD_NUMBER is on line 16, then this value should be 15
        const int LINE_NUMBER_TO_REPLACE            = 15;
        bool bFilesExist = File.Exists(GAME_VERSION_FILE) && File.Exists(GAME_BUILD_FILE);

        if (bFilesExist)
        {
            FileInfo GVFInfo = new FileInfo(GAME_VERSION_FILE);
            FileInfo GBFInfo = new FileInfo(GAME_BUILD_FILE);
            GVFInfo.IsReadOnly = false;
            GBFInfo.IsReadOnly = false;

            string GAME_BUILD_Text = File.ReadAllText(GAME_BUILD_FILE);
            int GAME_BUILD_Number = int.Parse(GAME_BUILD_Text);
            int NewNumber = GAME_BUILD_Number + 1;
            string GAME_VERSION_Text = File.ReadAllText(GAME_VERSION_FILE);

            if (GAME_VERSION_Text.Contains(BUILD_NUMBER_TEXT))
            {
                // Get the line numbers of GameVersion.h and add 1.
                int TotalLinesInGameVersionFile = File.ReadAllLines(GAME_VERSION_FILE).GetLength(0) + 1;

                using (var sr = new StreamReader(GAME_VERSION_FILE))
                {
                    for (int i = 1; i < TotalLinesInGameVersionFile; i++)
                    {
                        // If the line contains #define GAME_BUILD_NUMBER
                        if (sr.ReadLine().Contains(BUILD_NUMBER_TEXT))
                        {
                            // Optional: Write an output to Visual Studio Output
                            System.Console.WriteLine("New build number generated: " + NewNumber.ToString());
                            sr.Close();
                            var GameVersionFileLines = new List<string>(File.ReadAllLines(GAME_VERSION_FILE));
                            GameVersionFileLines.RemoveAt(LINE_NUMBER_TO_REPLACE);
                            GameVersionFileLines.Insert(LINE_NUMBER_TO_REPLACE, (BUILD_NUMBER_TEXT + " " + NewNumber));
                            File.WriteAllLines(GAME_VERSION_FILE, GameVersionFileLines);
                            File.WriteAllText(GAME_BUILD_FILE, NewNumber.ToString());
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            /* Optional: Comment out if you don't want compilation to fail */
            throw new BuildException("Failed to get GAME_VERSION_FILE or GAME_BUILD_FILE. Make sure they exist!");
        }

        DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "CppGame" } );
	}
}
