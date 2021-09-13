// Copyright Soren Gilbertson


#include "Axis3.h"
#include "Star.h"
#include "NiagaraActor.h"
#include "NiagaraComponent.h"
#include "OrbitDebugActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerInput.h"
#include "OverviewPlayer.h"
#include "CelestialGameMode.h"

// Sets default values
AAxis3::AAxis3()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	FCollisionResponseContainer CollisionResponce = FCollisionResponseContainer(ECollisionResponse::ECR_Ignore);
	CollisionResponce.GameTraceChannel2 = ECollisionResponse::ECR_Block;

	BoxX = CreateDefaultSubobject<UBoxComponent>("BoxX");
	BoxX->SetRelativeLocation(FVector(1100, 0, 0));
	BoxX->SetBoxExtent(FVector(1000, 50, 50), false);
	BoxX->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxX->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxX->SetCollisionResponseToChannels(CollisionResponce);
	BoxX->SetupAttachment(RootComponent);

	BoxY = CreateDefaultSubobject<UBoxComponent>("BoxY");
	BoxY->SetRelativeLocation(FVector(0, 1100, 0));
	BoxY->SetRelativeRotation(FRotator(0, 90, 0));
	BoxY->SetBoxExtent(FVector(1000, 50, 50), false);
	BoxY->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxY->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxY->SetCollisionResponseToChannels(CollisionResponce);
	BoxY->SetupAttachment(RootComponent);

	BoxZ = CreateDefaultSubobject<UBoxComponent>("BoxZ");
	BoxZ->SetRelativeLocation(FVector(0, 0, 1100));
	BoxZ->SetRelativeRotation(FRotator(90, 0, 0));
	BoxZ->SetBoxExtent(FVector(1000, 50, 50), false);
	BoxZ->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxZ->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxZ->SetCollisionResponseToChannels(CollisionResponce);
	BoxZ->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAxis3::BeginPlay()
{
	Super::BeginPlay();
	
	Cast<ACelestialGameMode>(GetWorld()->GetAuthGameMode())->OnPerspectiveChanged.AddDynamic(this, &AAxis3::SwitchVisibility);
}

// Called every frame
void AAxis3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Constant scale
	if (StaticMesh->IsVisible())
		SetActorScale3D(
			FVector((UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation() - GetActorLocation()).Size() / 10000)
		);

	// Click and drag check
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(FKey(EKeys::LeftMouseButton)) && bMouseDown == 0)
	{
		FHitResult Hit;
		GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery4, false, Hit);
		if (EnsureHit(Hit.bBlockingHit, Hit.Actor, Hit.Component))
			bMouseDown = 1;
		else
			bMouseDown = 255;
	}
	// Release mouse
	else if (!GetWorld()->GetFirstPlayerController()->IsInputKeyDown(FKey(EKeys::LeftMouseButton)) && bMouseDown == 1)
	{
		GetWorld()->GetFirstPlayerController()->GetPawn<AOverviewPlayer>()->bCanRot = true;
		BoxX->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxY->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxZ->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AOrbitDebugActor::Get()->DrawOrbits();
		bMouseDown = 0;
	}
	else if (!GetWorld()->GetFirstPlayerController()->IsInputKeyDown(FKey(EKeys::LeftMouseButton)) && bMouseDown > 1)
		bMouseDown = 0;

	// Moving the body
	if (bMouseDown == 1)
	{
		// Calculate the plane to move the actor along
		FVector Location;
		FVector Direction;
		GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(Location, Direction);
		FVector LineEnd = Location + (Direction * 100000000);
		float t;
		FVector Intersetcion;
		UKismetMathLibrary::LinePlaneIntersection_OriginNormal(
			UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation(),
			LineEnd,
			GetActorLocation(),
			Axis == 0 ? FVector::ZeroVector : Axis == 1 || Axis == 2 ? FVector(0, 0, 1) : FVector(1, 0, 0),
			t,
			Intersetcion
		);

		// Move the body to the new location
		FVector NewLocation =
			Intersetcion * (Axis == 0 ? FVector::ZeroVector : Axis == 1 ? FVector(1, 0, 0) : Axis == 2 ? FVector(0, 1, 0) : FVector(0, 0, 1)) +
			GetActorLocation() * (Axis == 0 ? FVector::ZeroVector : Axis == 1 ? FVector(0, 1, 1) : Axis == 2 ? FVector(1, 0, 1) : FVector(1, 1, 0));
		SetActorLocation(NewLocation);
		BodyToMove->SetActorLocation(NewLocation);

		// Update any Niagara particles
		if (ANiagaraActor* NiagaraActor = Cast<ANiagaraActor>(BodyToMove))
		{
			NiagaraActor->GetNiagaraComponent()->ReinitializeSystem();
			NiagaraActor->GetNiagaraComponent()->SetPaused(true);
		}
		else if (AStar* Star = Cast<AStar>(BodyToMove))
		{
			Star->GetParticleComp()->ReinitializeSystem();
			Star->GetParticleComp()->SetPaused(true);
		}
	}
}

bool AAxis3::EnsureHit(bool BlockingHit, TWeakObjectPtr<AActor> HitActor, TWeakObjectPtr<UPrimitiveComponent> HitComponent)
{
	if (BlockingHit && HitActor.IsValid() && HitActor == this)
	{
		if (HitComponent == BoxX)
			Axis = 1;
		else if (HitComponent == BoxY)
			Axis = 2;
		else if (HitComponent == BoxZ)
			Axis = 3;
		GetWorld()->GetFirstPlayerController()->GetPawn<AOverviewPlayer>()->bCanRot = false;
		BoxX->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BoxY->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BoxZ->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		return true;
	}
	else
		return false;
}

