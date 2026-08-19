// Copyright Soren Gilbertson


#include "CelestialPlayer.h"
#include "CelestialBody.h"
#include "CelestialGameMode.h"
#include "Blueprint\UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "BodySystemFunctionLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/InputDataConfig.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ACelestialPlayer::ACelestialPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
	bAllowChangePerspective = false;

	RootComponent = Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("RootCollider"));
	Collider->InitBoxExtent(FVector::ZeroVector);
	Collider->SetSimulatePhysics(true);
	Collider->SetAngularDamping(1);
	Collider->SetEnableGravity(false);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collider);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(Collider);
}

// Called when the game starts or when spawned
void ACelestialPlayer::BeginPlay()
{
	Super::BeginPlay();

	Collider->SetMassOverrideInKg(NAME_None, mass);
	Collider->OnComponentEndOverlap.AddDynamic(this, &ACelestialPlayer::OnOverlapEnd);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACelestialPlayer::OnOverlapBegin);

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
	
		LimitVelocity();
	}
}

void ACelestialPlayer::SetCurrentVelocity(FVector NewVelocity)
{
	currentVelocity = NewVelocity;
}

int ACelestialPlayer::GetMass() const { return mass; }

ACelestialBody* ACelestialPlayer::GetLargestForce()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		return nullptr;
	}

	// Sorts the map from high to low based on values
	ForcePerBody.ValueSort([](const float A, const float B) { return A > B; });

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
	FVector playerForwardDir = this->GetActorRotation().Vector();
	FVector playerRightDir = FRotationMatrix(this->GetActorRotation()).GetScaledAxis(EAxis::Y);
	
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

	return ClosestBody;
}

// Called to bind functionality to input
void ACelestialPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	gameMode = Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode());
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
 
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMappingContext, 1);
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    
	if(!CelestialInputConfig) return; // Don't wanna bind to invalid UInputActions!
    
	Input->BindAction(CelestialInputConfig->Move, ETriggerEvent::Triggered, this, &ACelestialPlayer::Move);
	Input->BindAction(CelestialInputConfig->Look, ETriggerEvent::Triggered, this, &ACelestialPlayer::Rotate);
	Input->BindAction(CelestialInputConfig->ChangeSpeed, ETriggerEvent::Triggered, this, &ACelestialPlayer::ChangeThrottle);
	Input->BindAction(CelestialInputConfig->SwitchPerspective, ETriggerEvent::Completed, this, &ACelestialPlayer::SwitchPerspective);
	Input->BindAction(CelestialInputConfig->IgnoreGravity, ETriggerEvent::Completed, this, &ACelestialPlayer::SwitchIgnoreGravity);
	Input->BindAction(CelestialInputConfig->FocusPlanet, ETriggerEvent::Completed, this, &ACelestialPlayer::SwitchFocusPlanet);
}

void ACelestialPlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) OverlappedActor = OtherActor;
		//if (GEngine) GEngine->AddOnScreenDebugMessage(0, 5, FColor::Green, *FString::Printf(TEXT("Player Col w/ %s"), *Cast<ACelestialBody>(OtherActor)->GetBodyName().ToString()));
}

void ACelestialPlayer::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherActor == OverlappedActor)) OverlappedActor = nullptr;
		//if (GEngine) GEngine->AddOnScreenDebugMessage(0, 5, FColor::Red, *FString::Printf(TEXT("Player End Col w/ %s"), *Cast<ACelestialBody>(OtherActor)->GetBodyName().ToString()));
}

void ACelestialPlayer::ChangeThrottle(const FInputActionValue& Value)
{
	if (Throttle + Value.Get<float>() / 10.f > 0.09f && Throttle + Value.Get<float>() / 10.f <= 5)
	{
		Throttle += Value.Get<float>() / 10.f;
	}
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
			FVector force = forceDir * ACelestialGameMode::GetGravitationalConstant() * mass * otherBody->GetMass() / sqrDst;
			FVector acceleration = force / mass;
			this->currentVelocity += acceleration * timeStep;

			if (ForcePerBody.Find(otherBody) != nullptr)
				ForcePerBody[otherBody] = force.Size();
			else
				ForcePerBody.Add(otherBody, force.Size());
		}
	}
	if (OverlappedActor && gameMode->GetCurrentPerspective() == 1)
	{
		FVector CollisionNormal = (this->GetActorLocation() - OverlappedActor->GetActorLocation()).GetUnsafeNormal();
		currentVelocity += (CollisionNormal * (currentVelocity.Size() / 1.5f));
	}
}

void ACelestialPlayer::UpdatePosition(float timeStep)
{
	this->SetActorLocation(FVector(this->GetActorLocation() + (currentVelocity * timeStep)));
}

void ACelestialPlayer::Move(const FInputActionValue& Value)
{
	if (Controller)
	{
		currentVelocity += (Camera->GetComponentRotation().RotateVector(Value.Get<FVector>()) * Throttle);
	}
}

void ACelestialPlayer::Rotate(const FInputActionValue& Value)
{
	if (Controller)
	{
		Collider->AddTorqueInDegrees(GetActorUpVector() * (Value.Get<FVector>().X * RotationForce), NAME_None, true);
		Collider->AddTorqueInDegrees(GetActorRightVector() * (Value.Get<FVector>().Y * RotationForce), NAME_None, true);
		Collider->AddTorqueInDegrees(GetActorForwardVector() * (Value.Get<FVector>().Z * RotationForce), NAME_None, true);
	}
}


void ACelestialPlayer::SwitchPerspective()
{
	if (bAllowChangePerspective) gameMode->SetPerspective(0);
}

void ACelestialPlayer::SwitchFocusPlanet()
{
	if (!GetWorld()->IsPaused())
		bFocusPlanet = !bFocusPlanet;
}
