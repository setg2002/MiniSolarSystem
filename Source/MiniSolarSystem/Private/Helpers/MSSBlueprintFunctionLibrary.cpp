// Copyright Soren Gilbertson


#include "Helpers/MSSBlueprintFunctionLibrary.h"
#include "Game/CelestialGameState.h"
#include "Game/CelestialGameMode.h"

ACelestialGameState* UMSSBlueprintFunctionLibrary::GetCelestialGameState(UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject))
		return nullptr;
	
	return WorldContextObject->GetWorld()->GetGameState<ACelestialGameState>();
}

ACelestialGameMode* UMSSBlueprintFunctionLibrary::GetCelestialGameMode(UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject))
		return nullptr;
	
	return WorldContextObject->GetWorld()->GetAuthGameMode<ACelestialGameMode>();
}
