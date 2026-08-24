// Copyright Soren Gilbertson


#include "MSSBlueprintFunctionLibrary.h"
#include "Game/CelestialGameState.h"

ACelestialGameState* UMSSBlueprintFunctionLibrary::GetCelestialGameState(UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject))
		return nullptr;
	
	return WorldContextObject->GetWorld()->GetGameState<ACelestialGameState>();
}
