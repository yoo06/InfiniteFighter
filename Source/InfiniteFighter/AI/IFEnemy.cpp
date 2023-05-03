// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IFEnemy.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AIFEnemy::AIFEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	// setting the mesh and animation
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SKM_MESH
	(TEXT("/Game/InFiniteFighter/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin"));
	if (SKM_MESH.Succeeded())
		GetMesh()->SetSkeletalMesh(SKM_MESH.Object);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>CHARACTER_ANIM
	(TEXT("/Game/InFiniteFighter/Characters/IFCharacterAnimBlueprint.IFCharacterAnimBlueprint_C"));
	if (CHARACTER_ANIM.Succeeded())
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT(""));
}

// Called when the game starts or when spawned
void AIFEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIFEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AIFEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

