// Copyright Soren Gilbertson


#include "BodySystemFunctionLibrary.h"
#include "CelestialGameMode.h"
#include "CelestialBody.h"

FBodySystem::FBodySystem(FName Name, TArray<ACelestialBody*> BodiesForIDs)
{
	SystemName = Name;
	for (ACelestialBody* Body : BodiesForIDs)
	{
		BodyIDs.Add(Body->GetID());
	}
}

void UBodySystemFunctionLibrary::AddIDs(FBodySystem& System, TArray<ACelestialBody*> IDsToAdd)
{
	for (ACelestialBody* Body : IDsToAdd)
	{
		System.BodyIDs.Add(Body->GetID());
	}
	ACelestialGameMode* GameMode = Cast<ACelestialGameMode>(IDsToAdd[0]->GetWorld()->GetAuthGameMode());
	GameMode->RemoveBodySystem(System);
	GameMode->AddBodySystem(System);
}

void UBodySystemFunctionLibrary::RemoveID(FBodySystem& System, ACelestialBody* IDsToRemove)
{
	System.BodyIDs.Remove(IDsToRemove->GetID());
	ACelestialGameMode* GameMode = Cast<ACelestialGameMode>(IDsToRemove->GetWorld()->GetAuthGameMode());
	GameMode->RemoveBodySystem(System);
	if (System.BodyIDs.Num() > 0)
		GameMode->AddBodySystem(System);
}

bool UBodySystemFunctionLibrary::DoesSystemContainBody(FBodySystem& System, ACelestialBody* Body)
{
	return System.BodyIDs.Contains(Body->GetID());
}
