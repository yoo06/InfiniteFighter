// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IFEnemy.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/IFCharacter.h"
#include "IFEnemyAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "IFEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	// setting the weapon
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>WEAPON_MESH
	(TEXT("/Game/InFiniteFighter/AI/mesh/SM_weapon_Stick.SM_weapon_Stick"));
	if (WEAPON_MESH.Succeeded())
		Weapon->SetStaticMesh(WEAPON_MESH.Object);

	Weapon->SetupAttachment(GetMesh(), TEXT("Weapon_Enemy"));
	Weapon->SetCollisionProfileName(TEXT("Weapon"));

	// setting the point and box for motion warping
	WarpPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WARP_POINT"));
	WarpPoint->SetRelativeLocation(FVector(75.0f, 0.0f, 0.0f));
	WarpPoint->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	WarpPoint->SetupAttachment(RootComponent);

	WarpCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WARP_COLLISION"));
	WarpCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	WarpCollision->SetBoxExtent(FVector(400.0f, 400.0f, 32.0f));
	WarpCollision->SetupAttachment(RootComponent);

	bCanBeAttacked = true;

	// setting Controller
	AIControllerClass = AIFEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;
}

// Called when the game starts or when spawned
void AIFEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AIFCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ensure(PlayerCharacter);

	AIFEnemyController* AIController = Cast<AIFEnemyController>(GetController());
	AIController->SetTarget(PlayerCharacter);

	PlayerCharacter->OnAttackEnd.AddUObject(this, &AIFEnemy::SetCanBeAttackedTrue);

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

void AIFEnemy::Attack()
{
	AnimInstance->PlayAttackMontage();
}

void AIFEnemy::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PlayerCharacter == OtherActor)
	{
		// If the Target variable is not already set
		if (PlayerCharacter->Target == nullptr)
		{
			// Set the player's Target variable to be this enemy
			PlayerCharacter->Target = this;
		}
	}
}

void AIFEnemy::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PlayerCharacter == OtherActor)
	{
		// If the Target variable is set to this
		if (PlayerCharacter->Target == this)
		{
			// Set the player's Target variable to be nullptr
			PlayerCharacter->Target = nullptr;
		}
	}
}

float AIFEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageCauser == PlayerCharacter)
	{
		if (bCanBeAttacked)
		{
			AnimInstance->React(this, DamageCauser);
			bCanBeAttacked = false;
			return DamageAmount;
		}
	}
	return DamageAmount;
}

void AIFEnemy::ActivateStun()
{
	AnimInstance->StopAllMontages(0);
	AnimInstance->SetStunState(true);
	GetWorld()->GetTimerManager().SetTimer(StunTimer, this, &AIFEnemy::DeactivateStun, 5.0f, false);
}

void AIFEnemy::DeactivateStun()
{
	AnimInstance->SetStunState(false);
}

bool AIFEnemy::GetStunState()
{
	return AnimInstance->GetStunState();
}

void AIFEnemy::SetDead()
{
	SetActorEnableCollision(false);
	AIFEnemyController* AIController = Cast<AIFEnemyController>(GetController());
	AIController->StopAI();
}

void AIFEnemy::PlayMontage(UAnimMontage* AnimMontage)
{
	AnimInstance->Montage_Play(AnimMontage);
}