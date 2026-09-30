import unreal
from typing import cast

UES = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)
if UES:
    world = UES.get_editor_world()
    if world:
        found_actor = unreal.GameplayStatics.get_actor_of_class(world, unreal.Skybox.static_class())
        skybox_actor = cast(unreal.Skybox, found_actor)
        skybox_actor.make_texture()
        print("Skybox texture made")
    else:
        unreal.log("Invalid World")
else:
    unreal.log("Invalid Subsystem")
