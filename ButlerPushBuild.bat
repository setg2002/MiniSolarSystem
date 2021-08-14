@echo off
REM #push build to itch.io using butler / refinery

REM ### fill out config settings ####
set pathToBuild=g:\Builds\SpaceGame\_BuildToPush
set butlerName=setg2002/MiniSolarSystem:win
set butlerNameDebug=setg2002/MiniSolarSystem:win-debug

REM ###run####

REM #compress the build to tmp zip for Butler use:
cd %pathToBuild%
7z a MiniSolarSystem.zip %pathToBuild%\*

REM #send file to itch.io via Butler:
cd g:\UnrealProjects\MiniSolarSystem\Source\MiniSolarSystem
butler push %pathToBuild%\MiniSolarSystem.zip %butlerNameDebug% --userversion-file GAME_BUILD.txt

REM #cleanup zip file:
del %pathToBuild%\MiniSolarSystem.zip

REM #compress the build to tmp zip for Butler use:
cd %pathToBuild%
del %pathToBuild%\WindowsNoEditor\MiniSolarSystem\Binaries\Win64\MiniSolarSystem.pdb /f /q
7z a MiniSolarSystem.zip %pathToBuild%\*

REM #send file to itch.io via Butler:
cd g:\UnrealProjects\MiniSolarSystem\Source\MiniSolarSystem
butler push %pathToBuild%\MiniSolarSystem.zip %butlerName% --userversion-file GAME_BUILD.txt

REM #cleanup zip file:
del %pathToBuild%\MiniSolarSystem.zip

pause