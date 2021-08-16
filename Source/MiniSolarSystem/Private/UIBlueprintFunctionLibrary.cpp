// Copyright Soren Gilbertson


#include "UIBlueprintFunctionLibrary.h"
#include "CelestialPlayer.h"
#include "CelestialBody.h"


FVector UUIBlueprintFunctionLibrary::GetRelativeVelocity(ACelestialPlayer* Player, ACelestialBody* Body)
{
    // Calculations from SolarSystem by Sebastian Lague

    FVector dirToPlanet = (Body->GetActorLocation() - Player->GetActorLocation()).GetSafeNormal();

    // Calculate horizontal/vertical axes relative to direction toward planet
    FVector horizontal = FVector::CrossProduct(dirToPlanet, Player->GetActorUpVector()).GetSafeNormal();
    horizontal *= FMath::Sin(FVector::DotProduct(horizontal, Player->GetActorRightVector())); // make sure roughly same direction as right vector of cam
    FVector vertical = FVector::CrossProduct(dirToPlanet, horizontal).GetSafeNormal();
    vertical *= FMath::Sign(FVector::DotProduct(vertical, Player->GetActorUpVector()));

    // Calculate relative velocity
    FVector relativeVelocityWorldSpace = Player->GetCurrentVelocity() - Body->GetCurrentVelocity();
    float vx = -FVector::DotProduct(relativeVelocityWorldSpace, horizontal);
    float vy = -FVector::DotProduct(relativeVelocityWorldSpace, vertical);
    float vz = FVector::DotProduct(relativeVelocityWorldSpace, dirToPlanet);
    return FVector(vx, vy, vz);
}
