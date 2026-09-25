# Mini Solar System
A simulation about procedurally generated planets and solar systems.

Roadmap: https://trello.com/b/sNuAdFSM/spacegame

Work Blog: https://sorengworklog.blogspot.com/


### Notes: ###
* Play mode needs to be "Standalone Game" to see loading screens


### Known Issues: ###
* If building from source results in an error "bulk data compressed header read error. this package may be corrupt!" then delete the procedural textures folder and try again. The textures can be regenerated in-engine by clicking 'Make Texture' on the BP_Skybox in the scene and 'New Variants' on the asteroid manager. The resulting textures can be applied in their reespective material graph.


### Engine ###
Must use custom engine fork for this project to compile. Repository can be found at [https://github.com/setg2002/UnrealEngine5_MiniSolarSystem](https://github.com/setg2002/UnrealEngine5_MiniSolarSystem)
