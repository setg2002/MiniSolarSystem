@echo off
REM #push build to itch.io using butler / refinery

REM ### fill out config settings ####
set pathToBuild=g:\Builds\SpaceGame\_BuildToPush
set butlerName=setg2002/MiniSolarSystem:win

REM ###run####

REM #compress the build to tmp zip for Butler use:
cd %pathToBuild%
7z a MiniSolarSystem.zip %pathToBuild%\*

REM #send file to itch.io via Butler:
cd g:\UnrealProjects\MiniSolarSystem\Source\MiniSolarSystem
butler push %pathToBuild%\MiniSolarSystem.zip %butlerName% --userversion-file GAME_BUILD.txt

REM #cleanup zip file:
del %pathToBuild%\MiniSolarSystem.zip
pause