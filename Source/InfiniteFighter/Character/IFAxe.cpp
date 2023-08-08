// Fill out your copyright notice in the Description page of Project Settings.


#include "IFAxe.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SphereComponent.h"
#include "IFCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/IFEnemy.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/DamageEvents.h"
#include "GameplayTags/AxeTag.h"
#include "NiagaraComponent.h"


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
	Axe = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AXE"));
	Axe->SetupAttachment(Lodge);
	Axe->SetRelativeLocation(FVector(-12.0f, 0.0f, -30.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_AXE
	(TEXT("/Game/InFiniteFighter/Weapon/axe.axe"));
	if (SK_AXE.Succeeded())
		Axe->SetSkeletalMesh(SK_AXE.Object);

	Axe->SetCollisionProfileName(TEXT("Weapon"));

	// Setting Particles
	TrailNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TRAIL_NIAGARA_COMPONENT"));
	TrailNiagaraComponent->SetupAttachment(Axe);
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>TRAIL_NIAGARA
	(TEXT("/Game/InFiniteFighter/FX/Trail/Recall-Trail_Niagara.Recall-Trail_Niagara"));
	if (TRAIL_NIAGARA.Succeeded())
		TrailNiagaraComponent->SetAsset(TRAIL_NIAGARA.Object);

	TrailNiagaraComponent->bAutoActivate = false;

	CatchParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CATCH_PARTICLE_COMPONENT"));
	CatchParticleComponent->SetupAttachment(Axe);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>CATCH_PARTICLE
	(TEXT("/Game/InFiniteFighter/FX/Leviathon/P_AxeCatch.P_AxeCatch"));
	if (CATCH_PARTICLE.Succeeded())
		CatchParticleComponent->SetTemplate(CATCH_PARTICLE.Object);

	CatchParticleComponent->SetActorParameter(TEXT("VertSurfaceActor"), this);
	CatchParticleComponent->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>SPARK_PARTICLE
	(TEXT("/Game/InFiniteFighter/FX/Leviathon/P_my_spark_burst.P_my_spark_burst"));
	if (SPARK_PARTICLE.Succeeded())
		SparkParticle = SPARK_PARTICLE.Object;

	// setting the projectile
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE_MOVEMENT"));
	ProjectileMovement->MaxSpeed		 = 4500.0f;
	ProjectileMovement->InitialSpeed	 = 3000.0f;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>AXE_GRAVITY_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/Curve/AxeGravityCurve.AxeGravityCurve"));
	if (AXE_GRAVITY_CURVE.Succeeded())
		AxeGravityCurveFloat = AXE_GRAVITY_CURVE.Object;

	AxeGravityTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AXE_GRAVITY_TIMELINE"));
	AxeGravityTimeline->SetTimelineLength(1.2f);

	static ConstructorHelpers::FObjectFinder<UCurveFloat>AXE_ROTATE_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/Curve/AxeRotateCurve.AxeRotateCurve"));
	if (AXE_ROTATE_CURVE.Succeeded())
		AxeRotateCurveFloat = AXE_ROTATE_CURVE.Object;

	AxeRotateTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AXE_ROTATE_TIMELINE"));
	AxeRotateTimeline->SetTimelineLength(1.0f);

	static ConstructorHelpers::FObjectFinder<UCurveFloat>WIGGLE_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/Curve/WiggleCurve.WiggleCurve"));
	if (WIGGLE_CURVE.Succeeded())
		WiggleCurveFloat = WIGGLE_CURVE.Object;

	WiggleTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("WIGGLE_TIMELINE"));
	WiggleTimeline->SetTimelineLength(0.8f);

	static ConstructorHelpers::FObjectFinder<UCurveFloat>RETURN_SPEED_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/Curve/ReturnSpeedCurve.ReturnSpeedCurve"));
	if (RETURN_SPEED_CURVE.Succeeded())
		ReturnSpeedCurveFloat = RETURN_SPEED_CURVE.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>RETURN_TILT_START_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/Curve/ReturnTiltStartCurve.ReturnTiltStartCurve"));
	if (RETURN_TILT_START_CURVE.Succeeded())
		ReturnTiltStartCurveFloat = RETURN_TILT_START_CURVE.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>RETURN_TILT_END_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/Curve/ReturnTiltEndCurve.ReturnTiltEndCurve"));
	if (RETURN_TILT_END_CURVE.Succeeded())
		ReturnTiltEndCurveFloat = RETURN_TILT_END_CURVE.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat>RIGHT_VECTOR_CURVE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/Curve/RightVectorCurve.RightVectorCurve"));
	if (RIGHT_VECTOR_CURVE.Succeeded())
		RightVectorCurveFloat = RIGHT_VECTOR_CURVE.Object;

	ReturnTiltStartTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RETURN_TILT_START_TIMELINE"));
	ReturnTiltEndTimeline   = CreateDefaultSubobject<UTimelineComponent>(TEXT("RETURN_TILT_END_TIMELINE"));
	ReturnSpeedTimeline     = CreateDefaultSubobject<UTimelineComponent>(TEXT("RETURN_SPEED_TIMELINE"));
	RightVectorTimeline     = CreateDefaultSubobject<UTimelineComponent>(TEXT("RIGHT_VECTOR_TIMELINE"));

	DistanceFromCharacter	  = 0;
	CameraLocation			  = FVector::ZeroVector;
	ReturnRightVector		  = FVector::ZeroVector;
	ReturnLocation			  = FVector::ZeroVector;
	ReturnStartLocation		  = FVector::ZeroVector;
	CameraRotation			  = FRotator::ZeroRotator;
	ReturnStartRotation		  = FRotator::ZeroRotator;
	ReturnStartCameraRotation = FRotator::ZeroRotator;
	TiltingRotation			  = FRotator::ZeroRotator;
	SpinCount				  = -1;

	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(Character);

	IdleTag		 = AXE_IDLE;
	FlyingTag	 = AXE_FLYING;
	LodgedTag	 = AXE_LODGED;
	ReturningTag = AXE_RETURNING;
}

// Called when the game starts or when spawned
void AIFAxe::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovement->Deactivate();

	Character = Cast<AIFCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	SetAxeState(IdleTag);
}

void AIFAxe::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Set gravity timeline when axe is thrown
	OnGravityTimelineFunction.BindDynamic(this, &AIFAxe::UpdateAxeGravity);
	AxeGravityTimeline->AddInterpFloat(AxeGravityCurveFloat, OnGravityTimelineFunction);

	// set the axe to rotate by desired time
	OnRotateTimelineFunction.BindDynamic(this, &AIFAxe::UpdateRotate);
	AxeRotateTimeline->AddInterpFloat(AxeRotateCurveFloat, OnRotateTimelineFunction);

	// called when timeline ends / function replacing loop
	OnRotateTimelineFinished.BindDynamic(this, &AIFAxe::SpinStop);
	AxeRotateTimeline->SetTimelineFinishedFunc(OnRotateTimelineFinished);

	// wiggling before the recall
	OnWiggleTimelineFunction.BindDynamic(this, &AIFAxe::UpdateWiggle);
	WiggleTimeline->AddInterpFloat(WiggleCurveFloat, OnWiggleTimelineFunction);

	// starts the recall function
	OnWiggleTimelineFinished.BindDynamic(this, &AIFAxe::RecallMovement);
	WiggleTimeline->SetTimelineFinishedFunc(OnWiggleTimelineFinished);

	// making the curve of the axe to the right when axe is recalling
    OnRightVectorTimelineFunction.BindDynamic(this, &AIFAxe::UpdateRightVector);
    RightVectorTimeline->AddInterpFloat(RightVectorCurveFloat, OnRightVectorTimelineFunction);

	// setting the speed of recall by distance
    OnReturnSpeedTimelineFunction.BindDynamic(this, &AIFAxe::UpdateReturnLocation);
    ReturnSpeedTimeline->AddInterpFloat(ReturnSpeedCurveFloat, OnReturnSpeedTimelineFunction);


	// tilting when recall starts
	OnReturnTiltStartTimelineFunction.BindDynamic(this, &AIFAxe::UpdateTiltStart);
	ReturnTiltEndTimeline->AddInterpFloat(ReturnTiltStartCurveFloat, OnReturnTiltStartTimelineFunction);

	// tilting when recall ends
    OnReturnTiltEndTimelineFunction.BindDynamic(this, &AIFAxe::UpdateTiltEnd);
    ReturnTiltStartTimeline->AddInterpFloat(ReturnTiltEndCurveFloat, OnReturnTiltEndTimelineFunction);

	// binds the axe to character
	OnReturnTimelineFinished.BindDynamic(this, &AIFAxe::CatchAxe);
	ReturnTiltEndTimeline->SetTimelineFinishedFunc(OnReturnTimelineFinished);
}

void AIFAxe::Throw()
{
	SetAxeState(FlyingTag);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// Get the current forward direction of the Axe
	FVector Direction = Character->GetCamera()->GetForwardVector();
	CameraLocation	  = Character->GetCamera()->GetComponentLocation();
	CameraRotation	  = Character->GetCamera()->GetComponentRotation();

	SetActorLocationAndRotation((Direction * 200.0f + CameraLocation) - Pivot->GetRelativeLocation(), CameraRotation);

	ProjectileMovement->Activate();

	AxeGravityTimeline->PlayFromStart();

	// Set the velocity of the ProjectileMovement component
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;

	AxeRotateTimeline->PlayFromStart();
	AxeRotateTimeline->SetPlayRate(2.5f);

	TrailNiagaraComponent->ActivateSystem();

	Character->OnAttackEnd.Broadcast();
}

void AIFAxe::Recall()
{
	Character->OnAttackEnd.Broadcast();

	if (HasMatchingGameplayTag(LodgedTag))
	{
		WiggleTimeline->PlayFromStart();
		WiggleTimeline->SetPlayRate(2.5f);
	}
	else if(HasMatchingGameplayTag(FlyingTag))
    {
        ProjectileMovement->Deactivate();
        AxeGravityTimeline->Stop();
        AxeRotateTimeline ->Stop();
		Pivot->SetRelativeRotation(FRotator::ZeroRotator);
		RecallMovement();
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
		GetActorLocation() + (GetVelocity().GetSafeNormal()) * 55.0f,
		ECollisionChannel::ECC_Visibility,
		Params
	);

    if (bResult)
    {
		LodgePosition(OutHit);
    }
// #if ENABLE_DRAW_DEBUG
// 		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (GetVelocity().GetSafeNormal()) * 55.0f, bResult? FColor::Green : FColor::Red, false, 2.0f);
// #endif
}

void AIFAxe::UpdateRotate(float InRotate)
{
	Pivot->SetRelativeRotation(FRotator(InRotate * -360.0f, 0.0f, 0.0f));
}

void AIFAxe::LodgePosition(const FHitResult& InHit)
{
	SetAxeState(LodgedTag);

	// Stopping the movements
	TrailNiagaraComponent->Deactivate();
	ProjectileMovement	 ->Deactivate();
	AxeGravityTimeline	 ->Stop();
	AxeRotateTimeline	 ->Stop();

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

	// when enemy is hitten
    if (InHit.BoneName != NAME_None)
    {
		AIFEnemy* TargetPawn = Cast<AIFEnemy>(InHit.GetActor());
		if (::IsValid(TargetPawn))
        {
            AttachToComponent(TargetPawn->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, InHit.BoneName);
			FDamageEvent DamageEvent;
			TargetPawn->TakeDamage(1, DamageEvent, Character->GetController(), this);
        }
    }
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkParticle, InHit.ImpactPoint, FRotator(AdjustPitch, 0.0f, AdjustRoll), true);
	}
}

void AIFAxe::UpdateWiggle(float InWigglePosition)
{
	float WiggleRoll = Lodge->GetRelativeRotation().Roll + (InWigglePosition * 12.0f);

	Lodge->SetRelativeRotation(FRotator(Lodge->GetRelativeRotation().Pitch, Lodge->GetRelativeRotation().Yaw, WiggleRoll));
}


void AIFAxe::RecallMovement()
{
	SetAxeState(ReturningTag);

	TrailNiagaraComponent->ActivateSystem();

	// Initialize components for recall
	DistanceFromCharacter	  = FMath::Clamp((GetActorLocation() - Character->GetMesh()->GetSocketLocation(TEXT("Weapon_R"))).Size(), 0, 3000);
	float TimelinePlayRate	  = FMath::Clamp(1400 / DistanceFromCharacter, 0.4f, 5.0f);
	ReturnStartLocation		  = GetActorLocation();
	ReturnStartRotation		  = GetActorRotation();
	ReturnStartCameraRotation = Character->GetCamera()->GetComponentRotation();
	Lodge->SetRelativeRotation(FRotator::ZeroRotator);

	// playing the timelines
	RightVectorTimeline->PlayFromStart();
	RightVectorTimeline->SetPlayRate(TimelinePlayRate);

	ReturnSpeedTimeline->PlayFromStart();
	ReturnSpeedTimeline->SetPlayRate(TimelinePlayRate);

	ReturnTiltStartTimeline->PlayFromStart();
	ReturnTiltStartTimeline->SetPlayRate(TimelinePlayRate);

	ReturnTiltEndTimeline->PlayFromStart();
	ReturnTiltEndTimeline->SetPlayRate(TimelinePlayRate);

	CalculateSpin(TimelinePlayRate);
}

void AIFAxe::UpdateRightVector(float InVector)
{
	// start position is designated, but the end position is dynamic
	ReturnRightVector = ((DistanceFromCharacter * InVector) * Character->GetCamera()->GetRightVector()) + Character->GetMesh()->GetSocketLocation(TEXT("Weapon_R"));
}

void AIFAxe::UpdateReturnLocation(float InSpeed)
{
	// setting the axe location by lerp
	ReturnLocation = FMath::Lerp(ReturnStartLocation, ReturnRightVector, InSpeed);
	SetActorLocation(ReturnLocation);

	FHitResult OutHit;

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		OutHit,
		GetActorLocation(),
		ReturnLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(25.0f),
		Params
	);

    if (bResult)
    {
        if (OutHit.BoneName != NAME_None)
        {
            AIFEnemy* TargetPawn = Cast<AIFEnemy>(OutHit.GetActor());
            if (::IsValid(TargetPawn))
            {
                FDamageEvent DamageEvent;
                TargetPawn->TakeDamage(1, DamageEvent, Character->GetController(), this);
            }
        }
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkParticle, ReturnLocation, FRotator::ZeroRotator, true);
		}
    }

 // #if ENABLE_DRAW_DEBUG
 // 		DrawDebugSphere(GetWorld(), (GetActorLocation() + ReturnLocation) / 2, 25.0f, 12, bResult? FColor::Green : FColor::Red, false, 2.0f);
 // #endif
}

void AIFAxe::UpdateTiltStart(float InValue)
{
	// Actual rotation will be done at TiltEnd
	FRotator TiltRotation = FRotator(ReturnStartCameraRotation.Pitch, ReturnStartCameraRotation.Yaw, ReturnStartCameraRotation.Roll + 60.0f);
	TiltingRotation       = FMath::Lerp(ReturnStartRotation, TiltRotation, InValue);
}

void AIFAxe::UpdateTiltEnd(float InValue)
{
	FRotator TargetRotation = FMath::Lerp(TiltingRotation, Character->GetMesh()->GetSocketRotation(TEXT("Weapon_R")), InValue);
	SetActorRotation(TargetRotation);
}

void AIFAxe::CatchAxe()
{
	SetAxeState(IdleTag);

	OnAxeCatch.ExecuteIfBound();
	TrailNiagaraComponent->Deactivate();

	CatchParticleComponent->Activate();
}

void AIFAxe::CalculateSpin(float InTimelinePlayRate)
{
	float AdjustRate = 1 / InTimelinePlayRate;
	SpinCount		 = FMath::RoundToInt(AdjustRate / 0.35);
	float SpinRate   = 1 / (AdjustRate / SpinCount);

	AxeRotateTimeline->SetPlayRate(SpinRate);
	AxeRotateTimeline->ReverseFromEnd();
}

void AIFAxe::SpinStop()
{
	if (SpinCount == 1)
	{
		AxeRotateTimeline->Stop();
		SpinCount = 0;
	}
	else if (SpinCount <= 0)
	{
		AxeRotateTimeline->PlayFromStart();
		SpinCount--;
	}
	else
	{
		AxeRotateTimeline->ReverseFromEnd();
		SpinCount--;
	}
}
