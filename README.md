# Mini Solar System
A game about procedurally generated planets and solar systems.

Uses the source build of the Unreal Engine, a guide to which can be found [here](https://docs.unrealengine.com/en-US/ProductionPipelines/DevelopmentSetup/BuildingUnrealEngine/index.html).

Roadmap: https://trello.com/b/sNuAdFSM/spacegame

Work Blog: https://sorengworklog.blogspot.com/


### Notes: ###
* Play mode needs to be "Standalone Game" to see loading screens


### Known Issues: ###
* If building from source results in an error "bulk data compressed header read error. this package may be corrupt!" then delete the procedural textures folder and try again. The textures can be regenerated in-engine by clicking 'Make Texture' on the BP_Skybox in the scene and 'New Variants' on the asteroid manager. The resulting textures can be applied in their reespective material graph.


### Changes To UE Source: ###
* In NiagaraDataInterfaceArrayFunctionLibrary.h all of the static functions need to be made public.
* In CurveLinearColor.h the variable FloatCurves needs to have the UPROPERTY specifier "SaveGame"
* In RichCurve.h the variables Time, Value, and Keys all need to have the UPROPERTY specifier "SaveGame"
* In DefaultGameMoviePlayer.cpp the entire body of OnAnyDown() has to be removed except for the return statement.
