// This is a copyright notice


#include "Skybox.h"

//MaterialInstanceConstant'/Game/MaterialStuff/GasGiantMat_Inst.GasGiantMat_Inst'

// Sets default values
ASkybox::ASkybox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/EngineSky/SM_SkySphere.SM_SkySphere'"), NULL, LOAD_None, NULL));
	Mesh->SetWorldScale3D(FVector(1000000));
	Mesh->SetupAttachment(GetRootComponent());
	DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/MaterialStuff/M_SkyBox_Inst.M_SkyBox_Inst'"), NULL, LOAD_None, NULL));
}

// Called when the game starts or when spawned
void ASkybox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkybox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

