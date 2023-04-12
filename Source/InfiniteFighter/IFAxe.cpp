// Fill out your copyright notice in the Description page of Project Settings.


#include "IFAxe.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "IFCharacterAnimInstance.h"

// Sets default values
AIFAxe::AIFAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Axe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AXE"));
	RootComponent = Axe;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE_MOVEMENT"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SK_AXE
	(TEXT("/Game/InFiniteFighter/Weapon/axe_low_scetchfab.axe_low_scetchfab"));
	if (SK_AXE.Succeeded())
		Axe->SetStaticMesh(SK_AXE.Object);

	Axe->SetCollisionProfileName(TEXT("NoCollision"));
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
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	// ProjectileMovement->MaxSpeed = 4500.0f;
	// ProjectileMovement->InitialSpeed = 3000.0f;
	UE_LOG(LogTemp, Warning, TEXT("Throw"));
}

