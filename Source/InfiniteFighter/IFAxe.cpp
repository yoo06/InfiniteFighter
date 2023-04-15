// Fill out your copyright notice in the Description page of Project Settings.


#include "IFAxe.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SphereComponent.h"
#include "IFCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AIFAxe::AIFAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = Root;

	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("PIVOT"));
	Pivot->SetupAttachment(Root);

	Lodge = CreateDefaultSubobject<USceneComponent>(TEXT("LODGE"));
	Lodge->SetupAttachment(Pivot);
	Lodge->SetRelativeLocation(FVector(12.0f, 0.0f, 35.0f));

	// Creating the Axe static Mesh
	Axe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AXE"));
	Axe->SetupAttachment(Lodge);
	Axe->SetRelativeLocation(FVector(-12.0f, 0.0f, -35.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SK_AXE
	(TEXT("/Game/InFiniteFighter/Weapon/axe_low_scetchfab.axe_low_scetchfab"));
	if (SK_AXE.Succeeded())
		Axe->SetStaticMesh(SK_AXE.Object);

	Axe->SetCollisionProfileName(TEXT("NoCollision"));

	// setting the projectile
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE_MOVEMENT"));
	ProjectileMovement->MaxSpeed		 = 4500.0f;
	ProjectileMovement->InitialSpeed	 = 3000.0f;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>AXE_GRAVITY_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/AxeGravityCurve.AxeGravityCurve"));
	if (AXE_GRAVITY_CURVE.Succeeded())
		AxeGravityCurveFloat = AXE_GRAVITY_CURVE.Object;

	AxeGravityTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AXE_GRAVITY_TIMELINE"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat>AXE_ROTATE_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/AxeRotateCurve.AxeRotateCurve"));
	if (AXE_ROTATE_CURVE.Succeeded())
		AxeRotateCurveFloat = AXE_ROTATE_CURVE.Object;

	AxeRotateTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AXE_ROTATE_TIMELINE"));

	CameraRotation = FRotator::ZeroRotator;
}

// Called when the game starts or when spawned
void AIFAxe::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovement->Deactivate();
	//RotateMovement->Deactivate();

	Character = Cast<AIFCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void AIFAxe::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AIFAxe::OnSphereComponentBeginOverlap);
	OnGravityTimelineFunction.BindDynamic(this, &AIFAxe::UpdateAxeGravity);
	AxeGravityTimeline->AddInterpFloat(AxeGravityCurveFloat, OnGravityTimelineFunction);

	OnRotateTimelineFunction.BindDynamic(this, &AIFAxe::UpdateRotateGravity);
	AxeRotateTimeline->AddInterpFloat(AxeRotateCurveFloat, OnRotateTimelineFunction);
}

void AIFAxe::Throw()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// Get the current forward direction of the Axe
	FVector  Direction		= Character->GetCamera()->GetForwardVector();
	FVector  CameraLocation = Character->GetCamera()->GetComponentLocation();
	CameraRotation = Character->GetCamera()->GetComponentRotation();

	SetActorLocationAndRotation((Direction * 200.0f + CameraLocation) - Pivot->GetRelativeLocation(), CameraRotation);

	ProjectileMovement->Activate();

	AxeGravityTimeline->PlayFromStart();

	// Set the velocity of the ProjectileMovement component
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;

	AxeRotateTimeline->Play();
	AxeRotateTimeline->SetLooping(true);
	AxeRotateTimeline->SetPlayRate(2.5f);
}

void AIFAxe::UpdateAxeGravity(float InGravity)
{
	ProjectileMovement->ProjectileGravityScale = InGravity;

    FHitResult OutHit;
	bool bResult = GetWorld()->LineTraceSingleByChannel
	(
		OutHit,
		GetActorLocation(),
		(GetVelocity().GetSafeNormal()) * 50.0f + GetActorLocation(),
		ECollisionChannel::ECC_Visibility
	);

    if (bResult)
    {
		LodgePosition(OutHit);
    }
#if ENABLE_DRAW_DEBUG
    DrawDebugLine(GetWorld(), GetActorLocation(), (GetVelocity().GetSafeNormal()) * 60.0f + GetActorLocation(), FColor::Red, false, 0, 1.0f, 5.0f);
#endif

}

void AIFAxe::UpdateRotateGravity(float InRotate)
{
	Pivot->SetRelativeRotation(FRotator(InRotate * -360.0f, 0.0f, 0.0f));
}

void AIFAxe::LodgePosition(const FHitResult& InHit)
{
	ProjectileMovement->Deactivate();
	AxeGravityTimeline->Stop();
	AxeRotateTimeline->Stop();
	Pivot->SetRelativeRotation(FRotator::ZeroRotator);
	SetActorRotation(CameraRotation);

	float AdjustRoll		  = FMath::FRandRange( -3.0f,  -8.0f);
	float InclineSurfaceRange = FMath::FRandRange(-30.0f, -55.0f);
	float RegularSurfaceRange = FMath::FRandRange( -5.0f, -25.0f);
	FMatrix RotateMatrix(InHit.ImpactNormal.GetSafeNormal(), FVector::ZeroVector, FVector::ZeroVector, FVector::ZeroVector);
	float MatrixPitch = RotateMatrix.Rotator().Pitch;

	float AdjustPitch = MatrixPitch > 0 ? InclineSurfaceRange - MatrixPitch : RegularSurfaceRange - MatrixPitch;

	Lodge->SetRelativeRotation(FRotator(AdjustPitch, 0.0f, AdjustRoll));

	float AdjustZ = MatrixPitch > 0 ? (90 - MatrixPitch) / 9 : 10;

	FVector AdjustLocation = InHit.ImpactPoint + FVector(0.0f, 0.0f, AdjustZ) + (GetActorLocation() - Lodge->GetComponentLocation());

	SetActorLocation(AdjustLocation);
}
