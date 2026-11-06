// Copyright Soren Gilbertson


#include "Helpers/MainMenuTextReader.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"


FText UMainMenuTextReader::GetText()
{
	FString FileString;
	FText Result = FText::FromString("ERROR");
	if (FFileHelper::LoadFileToString(FileString, *(FPaths::ProjectConfigDir() + "MenuText.ini")))
	{
		TArray<FString> Parsed;
		FileString.ParseIntoArray(Parsed, TEXT("\n"));
		Result = FText::FromString(Parsed[FMath::RandRange(0, Parsed.Num() - 1)]);
	}
	return Result;
}
