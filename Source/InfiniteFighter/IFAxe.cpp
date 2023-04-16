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
	Axe->SetRelativeLocation(FVector(-12.0f, 0.0f, -30.0f));

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
	AxeGravityTimeline->SetTimelineLength(1.2f);

	static ConstructorHelpers::FObjectFinder<UCurveFloat>AXE_ROTATE_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/AxeRotateCurve.AxeRotateCurve"));
	if (AXE_ROTATE_CURVE.Succeeded())
		AxeRotateCurveFloat = AXE_ROTATE_CURVE.Object;

	AxeRotateTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AXE_ROTATE_TIMELINE"));
	AxeGravityTimeline->SetTimelineLength(1.0f);

	static ConstructorHelpers::FObjectFinder<UCurveFloat>WIGGLE_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/WiggleCurve.WiggleCurve"));
	if (WIGGLE_CURVE.Succeeded())
		WiggleCurveFloat = WIGGLE_CURVE.Object;

	WiggleTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("WIGGLE_TIMELINE"));
	WiggleTimeline->SetTimelineLength(0.8f);

	static ConstructorHelpers::FObjectFinder<UCurveFloat>RETURN_SPEED_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/ReturnSpeedCurve.ReturnSpeedCurve"));
	if (RETURN_SPEED_CURVE.Succeeded())
		ReturnSpeedCurveFloat = RETURN_SPEED_CURVE.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>RETURN_TILT_START_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/ReturnTiltStartCurve.ReturnTiltStartCurve"));
	if (RETURN_TILT_START_CURVE.Succeeded())
		ReturnTiltStartCurveFloat = RETURN_TILT_START_CURVE.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>RETURN_TILT_END_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/ReturnTiltEndCurve.ReturnTiltEndCurve"));
	if (RETURN_TILT_END_CURVE.Succeeded())
		ReturnTiltEndCurveFloat = RETURN_TILT_END_CURVE.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>RIGHT_VECTOR_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/RightVectorCurve.RightVectorCurve"));
	if (RIGHT_VECTOR_CURVE.Succeeded())
		RightVectorCurveFloat = RIGHT_VECTOR_CURVE.Object;

	ReturnTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RETURN_TIMELINE"));

	CameraRotation			  = FRotator::ZeroRotator;
	DistanceFromCharacter	  = 0;
	ReturnRightVector		  = FVector::ZeroVector;
	ReturnLocation			  = FVector::ZeroVector;
	ReturnStartLocation		  = FVector::ZeroVector;
	ReturnStartRotation		  = FRotator::ZeroRotator;
	ReturnStartCameraRotation = FRotator::ZeroRotator;
	TiltingRotation			  = FRotator::ZeroRotator;
}

// Called when the game starts or when spawned
void AIFAxe::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovement->Deactivate();

	Character = Cast<AIFCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	SetAxeState(EAxeState::Idle);
}

void AIFAxe::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AIFAxe::OnSphereComponentBeginOverlap);
	OnGravityTimelineFunction.BindDynamic(this, &AIFAxe::UpdateAxeGravity);
	AxeGravityTimeline->AddInterpFloat(AxeGravityCurveFloat, OnGravityTimelineFunction);

	OnRotateTimelineFunction.BindDynamic(this, &AIFAxe::UpdateRotateGravity);
	AxeRotateTimeline->AddInterpFloat(AxeRotateCurveFloat, OnRotateTimelineFunction);

	OnWiggleTimelineFunction.BindDynamic(this, &AIFAxe::UpdateWiggle);
	WiggleTimeline->AddInterpFloat(WiggleCurveFloat, OnWiggleTimelineFunction);

	OnWiggleTimelineFinished.BindDynamic(this, &AIFAxe::RecallMovement);
	WiggleTimeline->SetTimelineFinishedFunc(OnWiggleTimelineFinished);

	OnReturnTimelineFunction.BindDynamic(this, &AIFAxe::UpdateTiltEnd);
	OnReturnTimelineFunction.BindDynamic(this, &AIFAxe::UpdateTiltStart);
	OnReturnTimelineFunction.BindDynamic(this, &AIFAxe::UpdateReturnLocation);
	OnReturnTimelineFunction.BindDynamic(this, &AIFAxe::UpdateRightVector);
	ReturnTimeline->AddInterpFloat(ReturnSpeedCurveFloat, OnReturnTimelineFunction);
	ReturnTimeline->AddInterpFloat(ReturnTiltEndCurveFloat, OnReturnTimelineFunction);
	ReturnTimeline->AddInterpFloat(ReturnTiltStartCurveFloat, OnReturnTimelineFunction);
	ReturnTimeline->AddInterpFloat(RightVectorCurveFloat, OnReturnTimelineFunction);

	OnReturnTimelineFinished.BindDynamic(this, &AIFAxe::CatchAxe);
	ReturnTimeline->SetTimelineFinishedFunc(OnReturnTimelineFinished);
}

void AIFAxe::Throw()
{
	SetAxeState(EAxeState::Flying);

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

void AIFAxe::Recall()
{
	switch (GetAxeState())
	{
	case EAxeState::Lodged:
		{
		WiggleTimeline->PlayFromStart();
		WiggleTimeline->SetPlayRate(2.5f);
		break;
		}
	case EAxeState::Flying:
		RecallMovement();
		break;
	default:
		return;
	}
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
}

void AIFAxe::UpdateRotateGravity(float InRotate)
{
	Pivot->SetRelativeRotation(FRotator(InRotate * -360.0f, 0.0f, 0.0f));
}

void AIFAxe::LodgePosition(const FHitResult& InHit)
{
	SetAxeState(EAxeState::Lodged);

	// Stopping the movement
	ProjectileMovement->Deactivate();
	AxeGravityTimeline->Stop();
	AxeRotateTimeline ->Stop();

	//resetting the rotation
	Pivot->SetRelativeRotation(FRotator::ZeroRotator);
	SetActorRotation(CameraRotation);

	float AdjustRoll		  = FMath::FRandRange( -3.0f,  -8.0f);
	float InclineSurfaceRange = FMath::FRandRange(-30.0f, -55.0f);
	float RegularSurfaceRange = FMath::FRandRange( -5.0f, -25.0f);

	// setting the rotation by using ImpactNormal position
	FMatrix RotateMatrix(InHit.ImpactNormal.GetSafeNormal(), FVector::ZeroVector, FVector::ZeroVector, FVector::ZeroVector);
	float MatrixPitch = RotateMatrix.Rotator().Pitch;

	float AdjustPitch = MatrixPitch > 0 ? InclineSurfaceRange - MatrixPitch : RegularSurfaceRange - MatrixPitch;

	// setting the rotation of lodge
	Lodge->SetRelativeRotation(FRotator(AdjustPitch, 0.0f, AdjustRoll));

	float AdjustZ = MatrixPitch > 0 ? (90 - MatrixPitch) / 9 : 10;

	FVector AdjustLocation = InHit.ImpactPoint + FVector(0.0f, 0.0f, AdjustZ) + (GetActorLocation() - Lodge->GetComponentLocation());

	SetActorLocation(AdjustLocation);
}

void AIFAxe::UpdateWiggle(float InWigglePosition)
{
	float WiggleRoll = Lodge->GetRelativeRotation().Roll + (InWigglePosition * 12.0f);

	Lodge->SetRelativeRotation(FRotator(Lodge->GetRelativeRotation().Pitch, Lodge->GetRelativeRotation().Yaw, WiggleRoll));
}


void AIFAxe::RecallMovement()
{
	SetAxeState(EAxeState::Returning);

	DistanceFromCharacter = FMath::Clamp((GetActorLocation() - Character->GetMesh()->GetSocketLocation(TEXT("Weapon_R"))).Size(), 0, 3000);
	float TimelinePlayRate = FMath::Clamp(1400 / DistanceFromCharacter, 0.4f, 0.7f);
	ReturnStartLocation = GetActorLocation();
	ReturnStartRotation = GetActorRotation();
	ReturnStartCameraRotation = Character->GetCamera()->GetComponentRotation();
	Lodge->SetRelativeRotation(FRotator::ZeroRotator);

	ReturnTimeline->PlayFromStart();
	ReturnTimeline->SetPlayRate(TimelinePlayRate);
}

void AIFAxe::UpdateRightVector(float InVector)
{
	ReturnRightVector = ((DistanceFromCharacter * InVector) * Character->GetCamera()->GetRightVector()) + Character->GetMesh()->GetSocketLocation(TEXT("Weapon_R"));
}

void AIFAxe::UpdateReturnLocation(float InSpeed)
{
	ReturnLocation = FMath::Lerp(ReturnStartLocation, ReturnRightVector, InSpeed);
	UE_LOG(LogTemp, Warning, TEXT("ReturnLocation : %s"), *ReturnLocation.ToString());
	SetActorLocation(ReturnLocation);
}

void AIFAxe::UpdateTiltStart(float InValue)
{
	FRotator TiltRotation = FRotator(ReturnStartCameraRotation.Pitch, ReturnStartCameraRotation.Yaw, ReturnStartCameraRotation.Roll + 60.0f);
	TiltingRotation = FMath::Lerp(ReturnStartRotation, TiltRotation, InValue);
}

void AIFAxe::UpdateTiltEnd(float InValue)
{
	FRotator TargetRotation = FMath::Lerp(TiltingRotation, Character->GetMesh()->GetSocketRotation(TEXT("Weapon_R")), InValue);
	SetActorRotation(TargetRotation);
}

void AIFAxe::CatchAxe()
{
	SetAxeState(EAxeState::Idle);
	OnAxeCatch.ExecuteIfBound();
}
