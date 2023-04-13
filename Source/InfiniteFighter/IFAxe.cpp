// Fill out your copyright notice in the Description page of Project Settings.


#include "IFAxe.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AIFAxe::AIFAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating the Axe static Mesh
	Axe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AXE"));
	RootComponent = Axe;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SK_AXE
	(TEXT("/Game/InFiniteFighter/Weapon/axe_low_scetchfab.axe_low_scetchfab"));
	if (SK_AXE.Succeeded())
		Axe->SetStaticMesh(SK_AXE.Object);

	Axe->SetCollisionProfileName(TEXT("NoCollision"));

	// Setting the SphereComponent
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE_COMPONENT"));
	SphereComponent->SetupAttachment(Axe);
	SphereComponent->InitSphereRadius(22.0f);
	SphereComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE_MOVEMENT"));
	ProjectileMovement->MaxSpeed = 4500.0f;
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->UpdatedComponent = Axe;

}

// Called when the game starts or when spawned
void AIFAxe::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovement->Deactivate();

}

// Called every frame
void AIFAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIFAxe::Throw()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// Get the current forward direction of the Axe
	FVector Direction = Axe->GetForwardVector();
	
	ProjectileMovement->Activate();

	// Set the velocity of the ProjectileMovement component to the direction times the initial speed
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
}
