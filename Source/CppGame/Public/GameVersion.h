// Copyright Soren Gilbertson

/********************************************************************/
/* DO NOT MODIFY LINE NUMBERS. THAT IS DON'T ADD NEW LINES OR REMOVE.
*  THIS FILE IS ACCESSED IN CppGameEditor.Target.cs FILE*/
/********************************************************************/

#pragma once
#include <string> 

#define GAME_MAJOR_VERSION 0
#define GAME_MINOR_VERSION 2
#define GAME_PATCH_VERSION 0

/*****Automatically modified after each build. Don't modify manually*****/
#define GAME_BUILD_NUMBER 671

#define GAME_VERSION_STRING        (std::to_string(GAME_MAJOR_VERSION) + "." + std::to_string(GAME_MINOR_VERSION) + "." + std::to_string(GAME_PATCH_VERSION) + "." + std::to_string(GAME_BUILD_NUMBER)).c_str()
#define GAME_VERSION_STRING_SHORT  (std::to_string(GAME_MAJOR_VERSION) + "." + std::to_string(GAME_MINOR_VERSION)).c_str()
