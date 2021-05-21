// Copyright Soren Gilbertson


#include "CelestialPlayer.h"
#include "CelestialBody.h"
#include "CelestialGameMode.h"
#include "Blueprint\UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACelestialPlayer::ACelestialPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->bUsePawnControlRotation = true;
	RootComponent = Camera;
}

// Called when the game starts or when spawned
void ACelestialPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	gameMode = Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode());

	for (auto& body : gameMode->GetBodies())
	{
		ForcePerBody.Add(body, 0);
	}
}

// Called every frame
void ACelestialPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller)
	{
		UpdatePosition(DeltaTime);

		// Camera Rotation Lag
		Controller->SetControlRotation(FMath::Lerp<FRotator>(Controller->GetControlRotation(), IntendedRotation, DeltaTime * RotationSpeed));
	
		LimitVelocity();
	}
}

FVector ACelestialPlayer::GetCurrentVelocity() const
{
	return currentVelocity;
}

int ACelestialPlayer::GetMass() const { return mass; }

ACelestialBody* ACelestialPlayer::GetLargestForce()
{
	// Sorts the map from high to low based on values
	ForcePerBody.ValueSort([](const float A, const float B) {
		return A > B; });

	for (auto& Pair : ForcePerBody)
	{
		return Pair.Key;
	}
	return nullptr;
}

ACelestialBody* ACelestialPlayer::LookingAtPlanet()
{
	static int AcceptanceAngle = 20;

	ACelestialBody* ClosestBody = nullptr;
	float dist = TNumericLimits<float>::Max();
	
	// Angle calculation from https://answers.unrealengine.com/questions/232851/computing-angle-between-forward-and-actor.html

	FVector playerLoc = this->GetActorLocation();
	FVector playerForwardDir = Controller->GetControlRotation().Vector();    // or any other way you want to get this - in any case, it's that actor's "forward" vector.
	FVector playerRightDir = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);    // or any other way you want to get this - in any case, it's that actor's "right" vector based on its forward vector
	
	for (auto& Body : gameMode->GetBodies())
	{
		FVector objectLoc = Body->GetActorLocation();

		// Calculate angles
		float dx = FVector::DotProduct((objectLoc - playerLoc), playerForwardDir);
		float dy = FVector::DotProduct((objectLoc - playerLoc), playerRightDir);
		// Convert to degrees and subtract orthogonal diff
		float angle = FMath::RadiansToDegrees(atan2(dx, dy)) - 90.0f;
		if ((angle < AcceptanceAngle && angle > -AcceptanceAngle) && (GetActorLocation() - Body->GetActorLocation()).Size() < dist)
		{
			ClosestBody = Body;
			dist = (GetActorLocation() - Body->GetActorLocation()).Size();
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s"), ClosestBody == nullptr ? TEXT("True") : TEXT("False"));
	return ClosestBody;
}

// Called to bind functionality to input
void ACelestialPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACelestialPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACelestialPlayer::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ACelestialPlayer::MoveUp);

	PlayerInputComponent->BindAxis("RotationX", this, &ACelestialPlayer::RotationX);
	PlayerInputComponent->BindAxis("RotationY", this, &ACelestialPlayer::RotationY);
	//PlayerInputComponent->BindAxis("RotationZ", this, &ACelestialPlayer::RotationZ);

	PlayerInputComponent->BindAxis("ChangeSpeed", this, &ACelestialPlayer::ChangeThrottle);

	PlayerInputComponent->BindAction("SwitchPerspective", EInputEvent::IE_Released, this, &ACelestialPlayer::SwitchPerspective);
	PlayerInputComponent->BindAction("IgnoreGravity", EInputEvent::IE_Released, this, &ACelestialPlayer::SwitchIgnoreGravity);
	PlayerInputComponent->BindAction("FocusPlanet", EInputEvent::IE_Released, this, &ACelestialPlayer::SwitchFocusPlanet);

}

void ACelestialPlayer::LimitVelocity()
{
	currentVelocity = UKismetMathLibrary::ClampVectorSize(currentVelocity, 0, MaxSpeed);
}

void ACelestialPlayer::UpdateVelocity(TArray<ACelestialBody*> allBodies, float timeStep)
{
	if (!bIgnoreGravity)
	{
		for (int i = 0; i < allBodies.Num(); i++)
		{
			ACelestialBody* otherBody = allBodies[i];

			float sqrDst = (otherBody->GetActorLocation() - this->GetActorLocation()).Size();
			FVector forceDir = (otherBody->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
			FVector force = forceDir * gameMode->gravitationalConstant * mass * otherBody->GetMass() / sqrDst;
			FVector acceleration = force / mass;
			this->currentVelocity += acceleration * timeStep;

			ForcePerBody[otherBody] = force.Size();
		}
	}
}

void ACelestialPlayer::UpdatePosition(float timeStep)
{
	this->SetActorLocation(FVector(this->GetActorLocation() + (currentVelocity * timeStep)));
}


void ACelestialPlayer::MoveForward(float AxisValue)
{
	if (Controller)
	{
		currentVelocity += (Controller->GetControlRotation().Vector() * AxisValue * Throttle);
	}
}

void ACelestialPlayer::MoveRight(float AxisValue)
{
	if (Controller)
	{
		currentVelocity += (UKismetMathLibrary::GetRightVector(Controller->GetControlRotation()) * AxisValue * Throttle);
	}
}

void ACelestialPlayer::MoveUp(float AxisValue)
{
	if (Controller)
	{
		currentVelocity += (UKismetMathLibrary::GetUpVector(Controller->GetControlRotation()) * AxisValue * Throttle);
	}
}


void ACelestialPlayer::SwitchPerspective()
{
	gameMode->SetPerspective(0);
}
