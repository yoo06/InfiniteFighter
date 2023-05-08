// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IFEnemy.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/IFCharacter.h"
#include "IFEnemyAnimInstance.h"

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

	static ConstructorHelpers::FClassFinder<UAnimInstance>ENEMY_ANIM
	(TEXT("/Game/InFiniteFighter/AI/EnemyAnimBlueprint.EnemyAnimBlueprint_C"));
	if (ENEMY_ANIM.Succeeded())
		GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	// setting the point and box for motion warping
	WarpPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WARP_POINT"));
	WarpPoint->SetRelativeLocation(FVector(75.0f, 0.0f, 0.0f));
	WarpPoint->SetupAttachment(RootComponent);

	WarpCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WARP_COLLISION"));
	WarpCollision->SetRelativeLocation(FVector(125.0f, 0.0f, 0.0f));
	WarpCollision->SetBoxExtent(FVector(75.0f, 75.0f, 32.0f));
	WarpCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AIFEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	WarpCollision->OnComponentBeginOverlap.AddDynamic(this, &AIFEnemy::OverlapBegin);
	WarpCollision->OnComponentEndOverlap.  AddDynamic(this, &AIFEnemy::OverlapEnd);
}

void AIFEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UIFEnemyAnimInstance>(GetMesh()->GetAnimInstance());
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

void AIFEnemy::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AIFCharacter* PlayerCharacter = Cast<AIFCharacter>(OtherActor);

	// If the cast is successful and the player's Target variable is not already set
	if (PlayerCharacter != nullptr && PlayerCharacter->Target == nullptr)
	{
		// Set the player's Target variable to be this enemy
		PlayerCharacter->Target = this;
	}
}

void AIFEnemy::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AIFCharacter* PlayerCharacter = Cast<AIFCharacter>(OtherActor);

	// If the cast is successful and the player's Target variable is set to this
	if (PlayerCharacter != nullptr && PlayerCharacter->Target == this)
	{
		// Set the player's Target variable to be nullptr
		PlayerCharacter->Target = nullptr;
	}
}

void AIFEnemy::PlayExecuteVictim()
{
	// set capsule to ignore pawn so it doesn't go throw floor
	GetCapsuleComponent()->SetCollisionProfileName("IgnoreOnlyPawn");
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// set mesh to overlap all so it doesn't block anything
	GetMesh()->SetCollisionProfileName("OverlapAll");
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AnimInstance->PlayExecuteVictimMontage();
}

