@echo off
REM #push build to itch.io using butler / refinery

REM ### fill out config settings ####
set pathToBuild=b:\MiniSolarSystem\Builds\SpaceGame\_BuildToPush
set butlerNameWin=setg2002/MiniSolarSystem:win
set butlerNameLnx=setg2002/MiniSolarSystem:linux

REM ###run####

REM #compress the build to tmp zip for Butler use:
cd %pathToBuild%

REM #delete debug files
del %pathToBuild%\Windows\MiniSolarSystem\Binaries\Win64\MiniSolarSystem-Win64-Shipping.pdb /f /q
del %pathToBuild%\Linux\MiniSolarSystem\Binaries\Linux\MiniSolarSystem-Linux-Shipping.debug /f /q

REM #Zip windows version
7z a MiniSolarSystemWin.zip %pathToBuild%\Windows\*

REM #send file to itch.io via Butler:
cd b:\MiniSolarSystem\Game\Source\MiniSolarSystem
butler push %pathToBuild%\MiniSolarSystemWin.zip %butlerNameWin% --userversion-file GAME_BUILD.txt

REM #cleanup zip file:
del %pathToBuild%\MiniSolarSystemWin.zip

REM #compress the build to tmp zip for Butler use:
cd %pathToBuild%
7z a MiniSolarSystemLnx.zip %pathToBuild%\Linux\*

REM #send file to itch.io via Butler:
cd b:\MiniSolarSystem\Game\Source\MiniSolarSystem
butler push %pathToBuild%\MiniSolarSystemLnx.zip %butlerNameLnx% --userversion-file GAME_BUILD.txt

REM #cleanup zip file:
del %pathToBuild%\MiniSolarSystemLnx.zip

pause