@echo off
REM #push build to itch.io using butler / refinery

REM ### fill out config settings ####
set pathToBuild=g:\Builds\SpaceGame\_BuildToPush
set butlerName=setg2002/space-game:win

REM ###run####

REM #compress the build to tmp zip for Butler use:
cd %pathToBuild%
7z a SpaceGame.zip %pathToBuild%\*

REM #send file to itch.io via Butler:
cd g:\UnrealProjects\CppGame\Source\CppGame
butler push %pathToBuild%\SpaceGame.zip %butlerName% --userversion-file GAME_BUILD.txt

REM #cleanup zip file:
del %pathToBuild%\SpaceGame.zip
pause