// Fill out your copyright notice in the Description page of Project Settings.


#include "IFCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "CommonInputSubsystem.h"
#include "Camera/CameraComponent.h"
#include "CineCameraActor.h"
#include "CineCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IFCharacterAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "IFAxe.h"
#include "UI/IFAimWidget.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "AI/IFEnemy.h"
#include "ExecutionAssetData.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameplayTags/AxeTag.h"
#include "GameplayTags/CharacterTag.h"
#include "GameplayTags/CharacterAnimationTag.h"
#include "GameplayTags/EnemyTag.h"


// Sets default values
AIFCharacter::AIFCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setting the inputs
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>IMC_DEFAULT
	(TEXT("/Game/InFiniteFighter/Input/IMC_Default.IMC_Default"));
	if (IMC_DEFAULT.Succeeded())
		DefaultContext = IMC_DEFAULT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOVE
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_Move.IA_Move"));
	if (IA_MOVE.Succeeded())
		MoveAction = IA_MOVE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_LOOK
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_Look.IA_Look"));
	if (IA_LOOK.Succeeded())
		LookAction = IA_LOOK.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_SPRINT
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_Sprint.IA_Sprint"));
	if (IA_SPRINT.Succeeded())
		SprintAction = IA_SPRINT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_DRAW_SHEATHE
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_Draw_Sheathe.IA_Draw_Sheathe"));
	if (IA_DRAW_SHEATHE.Succeeded())
		DrawSheatheAction = IA_DRAW_SHEATHE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_BLOCK
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_Block.IA_Block"));
	if (IA_BLOCK.Succeeded())
		BlockAction = IA_BLOCK.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_WEAKATTACK
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_WeakAttack.IA_WeakAttack"));
	if (IA_WEAKATTACK.Succeeded())
		WeakAttackAction = IA_WEAKATTACK.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_STRONGATTACK
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_StrongAttack.IA_StrongAttack"));
	if (IA_STRONGATTACK.Succeeded())
		StrongAttackAction = IA_STRONGATTACK.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_AIM
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_Aim.IA_Aim"));
	if (IA_AIM.Succeeded())
		AimAction = IA_AIM.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_THROW
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_Throw.IA_Throw"));
	if (IA_THROW.Succeeded())
		ThrowAction = IA_THROW.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_RECALL
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_Recall.IA_Recall"));
	if (IA_RECALL.Succeeded())
		RecallAction = IA_RECALL.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_EVADE
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_Evade.IA_Evade"));
	if (IA_EVADE.Succeeded())
		EvadeAction = IA_EVADE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_EXECUTE
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_Execute.IA_Execute"));
	if (IA_EXECUTE.Succeeded())
		ExecuteAction = IA_EXECUTE.Object;

	// Setting properties for Aiming the Axe
	static ConstructorHelpers::FObjectFinder<UCurveFloat>AIM_CURVE_FLOAT
	(TEXT("/Game/InFiniteFighter/Miscellaneous/Curve/AimCurve.AimCurve"));
	if (AIM_CURVE_FLOAT.Succeeded())
		AimCurveFloat = AIM_CURVE_FLOAT.Object;

	static ConstructorHelpers::FClassFinder<UIFAimWidget>AIM_HUD_C
	(TEXT("/Game/InFiniteFighter/Widget/Aim/Aim_Hud.Aim_Hud_C"));
	if (AIM_HUD_C.Succeeded())
		AimHUDClass = AIM_HUD_C.Class;

	AimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AIM_TIMELINE"));

	WarpCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WARP_COLLISION"));
	WarpCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	WarpCollision->SetBoxExtent(FVector(400.0f, 400.0f, 32.0f));
	WarpCollision->SetupAttachment(RootComponent);

	// creating parts for character (springarm, camera)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING_ARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);

	// positioning the skeletal mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MOTION_WARPING_COMPONENT"));

	// setting spring arm
	SpringArm->ProbeSize                = 16.0f;
	SpringArm->TargetArmLength          = 200.0f;
	SpringArm->SocketOffset             = FVector(0.0f, 50.0f, 80.0f);
	SpringArm->bUsePawnControlRotation  = true;
	SpringArm->bInheritPitch		    = true;
	SpringArm->bInheritYaw			    = true;
	SpringArm->bInheritRoll			    = true;
	// enabling camera lag for a smoother look
	SpringArm->bEnableCameraLag		    = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagMaxDistance     = 5.0f;
	SpringArm->CameraLagSpeed           = 2.0f;
	SpringArm->CameraRotationLagSpeed   = 10.0f;
	bUseControllerRotationYaw           = false;

	// setting character movement
	GetCharacterMovement()->MaxWalkSpeed			   = 400.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 50.0f;
	GetCharacterMovement()->RotationRate			   = FRotator(0.0f, 360.0f, 0.0f);

	// setting the mesh and animation
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SKM_MESH
	(TEXT("/Game/InFiniteFighter/Characters/Kratos/Kratos_Fix.Kratos_Fix"));
	if (SKM_MESH.Succeeded())
		GetMesh()->SetSkeletalMesh(SKM_MESH.Object);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>CHARACTER_ANIM
	(TEXT("/Game/InFiniteFighter/Characters/IFCharacterAnimBlueprint.IFCharacterAnimBlueprint_C"));
	if (CHARACTER_ANIM.Succeeded())
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));

	MovementVector = FVector2D::ZeroVector;
	Target == nullptr;

	// setting execution
	static ConstructorHelpers::FObjectFinder<UExecutionAssetData>SWEEP_REF
	(TEXT("/Game/InFiniteFighter/Miscellaneous/DataAsset/SweepExecution.SweepExecution"));
	if (SWEEP_REF.Succeeded())
		ExecutionArray.Add(SWEEP_REF.Object);

	static ConstructorHelpers::FObjectFinder<UExecutionAssetData>TACKLE_REF
	(TEXT("/Game/InFiniteFighter/Miscellaneous/DataAsset/TackleExecution.TackleExecution"));
	if (TACKLE_REF.Succeeded())
		ExecutionArray.Add(TACKLE_REF.Object);

	static ConstructorHelpers::FObjectFinder<UExecutionAssetData>SLAM_REF
	(TEXT("/Game/InFiniteFighter/Miscellaneous/DataAsset/SlamExecution.SlamExecution"));
	if (SLAM_REF.Succeeded())
		ExecutionArray.Add(SLAM_REF.Object);

	// setting Particle
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PARRYING_PARTICLE
	(TEXT("/Game/InFiniteFighter/FX/Leviathon/P_Parring_Spark.P_Parring_Spark"));
	if (PARRYING_PARTICLE.Succeeded())
		ParryingParticle = PARRYING_PARTICLE.Object;

	// setting CameraShake
	static ConstructorHelpers::FClassFinder<UCameraShakeBase>CAMERA_SHAKE
	(TEXT("/Game/InFiniteFighter/Miscellaneous/CameraShake/CS_Catch.CS_Catch_C"));
	if (CAMERA_SHAKE.Succeeded())
		CameraShake = CAMERA_SHAKE.Class;

	IdleState	  = CHARACTER_IDLE;
	AimState	  = CHARACTER_AIM;
	SprintState   = CHARACTER_SPRINT;
	DamagedState  = CHARACTER_DAMAGED;
	ParryingState = CHARACTER_PARRYING;
	BlockingState = CHARACTER_BLOCKING;
}

// Called when the game starts or when spawned
void AIFCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Adding mapping context to Enhanced Input Subsystem
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			SubSystem->AddMappingContext(DefaultContext, 0);

		InputSubsystem = ULocalPlayer::GetSubsystem<UCommonInputSubsystem>(PlayerController->GetLocalPlayer());
	}
	
	// Clamping Camera angle
	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (::IsValid(PlayerCameraManager))
	{
		PlayerCameraManager->ViewPitchMin = -60.0f;
		PlayerCameraManager->ViewPitchMax =  60.0f;
	}

	RotateDefault();

	AimHUD = CreateWidget<UIFAimWidget>(GetWorld()->GetFirstPlayerController(), AimHUDClass);

	CharacterState.AddTag(IdleState);
	CharacterState.AddTag(DamagedState);
}

// Called every frame
void AIFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AIFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,	       ETriggerEvent::Triggered, this, &AIFCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction,	       ETriggerEvent::Triggered, this, &AIFCharacter::Look);
		EnhancedInputComponent->BindAction(SprintAction,       ETriggerEvent::Triggered, this, &AIFCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction,       ETriggerEvent::Completed, this, &AIFCharacter::SprintEnd);
		EnhancedInputComponent->BindAction(DrawSheatheAction,  ETriggerEvent::Triggered, this, &AIFCharacter::DrawSheathe);
		EnhancedInputComponent->BindAction(BlockAction,        ETriggerEvent::Triggered, this, &AIFCharacter::BlockStart);
		EnhancedInputComponent->BindAction(BlockAction,        ETriggerEvent::Completed, this, &AIFCharacter::BlockEnd);
		EnhancedInputComponent->BindAction(WeakAttackAction,   ETriggerEvent::Triggered, this, &AIFCharacter::WeakAttack);
		EnhancedInputComponent->BindAction(StrongAttackAction, ETriggerEvent::Triggered, this, &AIFCharacter::StrongAttack);
		EnhancedInputComponent->BindAction(AimAction,		   ETriggerEvent::Triggered, this, &AIFCharacter::AimStart);
		EnhancedInputComponent->BindAction(AimAction,		   ETriggerEvent::Completed, this, &AIFCharacter::AimEnd);
		EnhancedInputComponent->BindAction(ThrowAction,		   ETriggerEvent::Triggered, this, &AIFCharacter::Throw);
		EnhancedInputComponent->BindAction(RecallAction,	   ETriggerEvent::Triggered, this, &AIFCharacter::RecallAxe);
		EnhancedInputComponent->BindAction(EvadeAction,		   ETriggerEvent::Triggered, this, &AIFCharacter::Evade);
		EnhancedInputComponent->BindAction(ExecuteAction,	   ETriggerEvent::Triggered, this, &AIFCharacter::Execute);
	}
}

void AIFCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Creating the Axe and Attaching it to Back
	FName BackSocket(TEXT("Weapon_Back"));
	Axe = GetWorld()->SpawnActor<AIFAxe>(FVector::ZeroVector, FRotator::ZeroRotator);

	if (::IsValid(Axe))
		Axe->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, BackSocket);

	// Creating CineCamera and Attaching it to Character
	CineCamera = GetWorld()->SpawnActor<ACineCameraActor>(GetActorLocation() + FVector(-210.0f, 50.0f, 80.0f), GetActorRotation());
	
	CineCamera->GetCineCameraComponent()->SetCurrentFocalLength(25.0f);
	CineCamera->GetCineCameraComponent()->SetFilmbackPresetByName(FString::Printf(TEXT("16:9 Digital Film")));
	
	if (::IsValid(CineCamera))
	{
		CineCamera->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}

	// cast IFCharacterAnimInstance to Character's AnimInstance
	AnimInstance = Cast<UIFCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	// Binding the Delegates
	AnimInstance->OnDraw.		   BindUObject(this, &AIFCharacter::Draw);
	AnimInstance->OnSheathe.	   BindUObject(this, &AIFCharacter::Sheathe);
	AnimInstance->OnCharacterMove. BindUObject(this, &AIFCharacter::RotateToCamera);
	AnimInstance->OnCharacterStop. BindUObject(this, &AIFCharacter::RotateDefault);
	AnimInstance->OnMontageStarted.AddDynamic (this, &AIFCharacter::RotateToCameraMontage);
	AnimInstance->OnMontageEnded.  AddDynamic (this, &AIFCharacter::RotateDefaultMontage);
	AnimInstance->OnThrow.		   BindUObject(Axe,  &AIFAxe::      Throw);
	Axe			->OnAxeCatch.	   BindUObject(this, &AIFCharacter::CatchAxe);
	AnimInstance->OnCatchEnd.	   BindLambda([this] { Axe->SetActorRelativeLocation(FVector::ZeroVector); });
	AnimInstance->OnParryingEnd.   BindLambda([this] 
	{ 
		CharacterState.RemoveTag(ParryingState);
		CharacterState.AddTag(BlockingState);
	});

	OnExecutionEnd.AddLambda([this] { CharacterState.AddTag(DamagedState); });
	
	OnAimTimelineFunction.BindDynamic(this, &AIFCharacter::UpdateAimCamera);
	AimTimeline->AddInterpFloat(AimCurveFloat, OnAimTimelineFunction);
	
	for(const auto& ExecutionAssetData : ExecutionArray)
		ExecutionAssetData->CreateSequencePlayer(CineCamera);
}

float AIFCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (CharacterState.HasTagExact(DamagedState))
	{
		if (CharacterState.HasTagExact(ParryingState))
		{
			auto Enemy = Cast<AIFEnemy>(DamageCauser);
			if (::IsValid(Enemy))
			{
				// set timer
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25f);
				GetWorld()->GetTimerManager().SetTimer(SlowTimer, [this]() { UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1); }, 0.05f, false);
				
				// set the animation and particle
				Enemy->ActivateStun();
				Parrying();
				FVector OffSet = (GetActorForwardVector() * 70) + (GetActorRightVector() * -20) + (GetActorUpVector() * 70);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParryingParticle, GetActorLocation() + OffSet, FRotator::ZeroRotator, true);
				Enemy->TakeDamage(0, DamageEvent, GetController(), this);

				return 0.0f;
			}
		}
		else
		{
			if (CharacterState.HasTagExact(BlockingState))
			{
				float DotProduct = FVector::DotProduct(GetActorForwardVector(), (DamageCauser->GetActorLocation() - GetActorLocation()).GetSafeNormal());
				if (DotProduct > 0)
				{
					AnimInstance->PlayBackDownMontage();
					return 0.0f;
				}
			}
			SetCameraShake();
			CharacterState.RemoveTag(DamagedState);
			GetWorld()->GetTimerManager().SetTimer(DamageTimer, [this]() { CharacterState.AddTag(DamagedState); }, 0.1f, false);
			AnimInstance->React(this, DamageCauser);
			return DamageAmount;
		}
	}
	return 0.0f;
}

void AIFCharacter::SetCameraShake()
{
	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (::IsValid(PlayerCameraManager))
	{
		PlayerCameraManager->StartCameraShake(CameraShake);
	}
}

AActor* AIFCharacter::GetAxe()
{
	return Axe;
}

void AIFCharacter::Move(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();

	if (::IsValid(Controller))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// scale movement input based on desired speed
		const FVector ScaledMovementInput = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X).GetSafeNormal();

		// add movement checking the type of input
		ECommonInputType CurrentInputType = InputSubsystem->GetCurrentInputType();

		if (CurrentInputType == ECommonInputType::MouseAndKeyboard) 
			AddMovementInput(ScaledMovementInput);
		else if (CurrentInputType == ECommonInputType::Gamepad)
			AddMovementInput(ScaledMovementInput, ((MovementVector.Size() / 50) < 0.7f) ? 0.4f : 1.0f);
	}
}

void AIFCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Adding Camera movement
	if (::IsValid(Controller))
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AIFCharacter::SprintStart()
{
	FVector2D MovementVectorNormal = MovementVector.GetSafeNormal();

	if (!CharacterState.HasTagExact(SprintState) && (MovementVectorNormal.X >= -0.5 && MovementVectorNormal.X < 0.5 && MovementVectorNormal.Y > 0))
	{
		CharacterState.AddTag(SprintState);
		CharacterState.RemoveTag(IdleState);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else if (!CharacterState.HasTagExact(IdleState) && !(MovementVectorNormal.X >= -0.5 && MovementVectorNormal.X < 0.5 && MovementVectorNormal.Y > 0))
	{
		CharacterState.AddTag(IdleState);
		CharacterState.RemoveTag(SprintState);
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	}
}

void AIFCharacter::SprintEnd()
{
	CharacterState.AddTag(IdleState);
	CharacterState.RemoveTag(SprintState);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void AIFCharacter::DrawSheathe()
{
	if(Axe->HasMatchingGameplayTag(AXE_IDLE))
		AnimInstance->PlayDrawSheatheMontage();
}

void AIFCharacter::Parrying()
{
	AnimInstance->PlayParryingMontage();
	CharacterState.RemoveTag(BlockingState);
}

void AIFCharacter::BlockStart()
{
	if (CharacterState.HasTagExact(IdleState))
	{
		CharacterState.RemoveTag(IdleState);
		CharacterState.AddTag(ParryingState);
		GetCharacterMovement()->MaxWalkSpeed  = 200.0f;
		AnimInstance->AnimState.AddTag(ANIM_BLOCK);
	}
}

void AIFCharacter::BlockEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	CharacterState.RemoveTag(BlockingState);
	CharacterState.AddTag(IdleState);
	AnimInstance->AnimState.RemoveTag(ANIM_BLOCK);
}

void AIFCharacter::WeakAttack()
{
	TSet<AActor*> Enemies;
	GetOverlappingActors(Enemies, AIFEnemy::StaticClass());
	if (Enemies.Num())
	{
		for (const auto& Enemy : Enemies)
		{
			// check if enemy is in character's sight (DotProduct on chracter's forward vector and character to enemy vector)
			float DotProduct = FVector::DotProduct(GetActorForwardVector(), (Enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal());
			
			if (DotProduct > 0.4)
			{
				// set the closest Enemy to Target
				Target = Cast<AIFEnemy>(Enemy);
				if(GetDistanceTo(Enemy) <= GetDistanceTo(Target))
					Target = Cast<AIFEnemy>(Enemy);
			}
		}
		if (::IsValid(Target))
		{
			Target->WarpPoint->SetWorldLocation(Target->GetActorLocation() + (Camera->GetComponentLocation() - Target->GetActorLocation()).GetSafeNormal() * 75);
			FVector Direction = Target->WarpPoint->GetComponentLocation() - Camera->GetComponentLocation();
			FRotator TargetRotation = Direction.Rotation();
			Controller->SetControlRotation(FRotator(Controller->GetControlRotation().Pitch, TargetRotation.Yaw, Controller->GetControlRotation().Roll));
		}
	}
	Target = nullptr;
	AnimInstance->PlayWeakAttackMontage();
}

void AIFCharacter::StrongAttack()
{
	TSet<AActor*> Enemies;
	GetOverlappingActors(Enemies, AIFEnemy::StaticClass());
	if (Enemies.Num())
	{
		for (const auto& Enemy : Enemies)
		{
			// check if enemy is in character's sight (DotProduct on chracter's forward vector and character to enemy vector)
			float DotProduct = FVector::DotProduct(GetActorForwardVector(), (Enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal());
			
			if (DotProduct > 0.4)
			{
				Target = Cast<AIFEnemy>(Enemy);
				if (GetDistanceTo(Enemy) <= GetDistanceTo(Target))
					Target = Cast<AIFEnemy>(Enemy);
			}
		}
		if (::IsValid(Target))
		{
			Target->WarpPoint->SetWorldLocation(Target->GetActorLocation() + (Camera->GetComponentLocation() - Target->GetActorLocation()).GetSafeNormal() * 75);
			FVector Direction = Target->WarpPoint->GetComponentLocation() - Camera->GetComponentLocation();
			FRotator TargetRotation = Direction.Rotation();
			Controller->SetControlRotation(FRotator(Controller->GetControlRotation().Pitch, TargetRotation.Yaw, Controller->GetControlRotation().Roll));
		}
	}
	Target = nullptr;
	AnimInstance->PlayStrongAttackMontage();
}

void AIFCharacter::AimStart()
{
	RotateToCamera();

	if (CharacterState.HasTagExact(IdleState))
	{
		CharacterState.RemoveTag(IdleState);
		CharacterState.AddTag(AimState);
		AimHUD->AddToViewport();
		AnimInstance->AnimState.AddTag(ANIM_AIM);
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		AimTimeline->Play();
		AimTimeline->SetPlayRate(2.0f);
	}

    FVector StartLocation = Camera->GetComponentLocation() + Camera->GetComponentRotation().Vector() * 100;
    FVector EndLocation   = Camera->GetComponentLocation() + Camera->GetComponentRotation().Vector() * 3000;

    FHitResult HitResult;

    bool bResult = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility);

	if (bResult)
	{
		if (AimHUD->GetAimTargetUI())
			AimHUD->SetAimTargetUI(true);
	}
	else
	{
		if (!AimHUD->GetAimTargetUI())
			AimHUD->SetAimTargetUI(false);
	}

}

void AIFCharacter::AimEnd()
{
	if(GetVelocity().Size() == 0) RotateDefault();

	AimHUD->RemoveFromParent();

	CharacterState.RemoveTag(AimState);
	CharacterState.AddTag(IdleState);
	AnimInstance->AnimState.RemoveTag(ANIM_AIM);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	AimTimeline->Reverse();
	AimTimeline->SetPlayRate(0.6f);
}

void AIFCharacter::Evade()
{
	AnimInstance->PlayDodgeMontage(MovementVector.GetSafeNormal());
}

void AIFCharacter::Execute()
{
	TSet<AActor*> Enemies;
	GetOverlappingActors(Enemies, AIFEnemy::StaticClass());
	if (Enemies.Num())
	{
		for (auto& Enemy : Enemies)
		{
			// check if character and enemy are facing(DotProduct on both character's forward vector)
			float DotProduct = FVector::DotProduct(GetActorForwardVector(), Enemy->GetActorForwardVector());
			auto TargetRef = Cast<AIFEnemy>(Enemy);
			if (DotProduct < 0 && TargetRef->HasMatchingGameplayTag(ENEMY_STUN))
			{
				Target = TargetRef;

				const int RandNum = FMath::RandRange(0, ExecutionArray.Num() - 1);

				const auto& ExecutionAssetData = ExecutionArray[RandNum];

				// Set Axe to Character's Back
				Sheathe();
				AnimInstance->AnimState.RemoveTag(ANIM_AXEHOLDING);
				AnimInstance->AnimState.RemoveTag(ANIM_DRAW);

				// Set MotionWarping position and warp
				Target->WarpPoint->SetRelativeLocation(ExecutionAssetData->WarpPoint);
				MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(TEXT("Target"), Target->WarpPoint->GetComponentTransform());

				// Play the montage
				AnimInstance->Montage_Play(ExecutionAssetData->AttackMontage);
				Target->PlayExecution(ExecutionAssetData->VictimMontage);

				// Reset the camera to center and play sequence
				bUseControllerRotationYaw = false;
				Controller->SetControlRotation(Target->WarpPoint->GetComponentRotation());
				ExecutionAssetData->Play();

				Target->SetDead(6);
				Target = nullptr;

				CharacterState.RemoveTag(DamagedState);
				GetWorld()->GetTimerManager().SetTimer(DamageTimer, [this]() { OnExecutionEnd.Broadcast(); }, ExecutionAssetData->Time, false);

				return;
			}
		}
	}
}

void AIFCharacter::UpdateAimCamera(float NewArmLength)
{
	SpringArm->TargetArmLength = NewArmLength;
}

void AIFCharacter::Throw()
{
	if(!AnimInstance->IsAnyMontagePlaying())
		AnimInstance->PlayThrowMontage();
}

void AIFCharacter::Draw()
{
	FName WeaponSocket(TEXT("Weapon_R"));

	Axe->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	Axe->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
}

void AIFCharacter::Sheathe()
{
	FName BackSocket(TEXT("Weapon_Back"));

	Axe->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	Axe->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, BackSocket);
}

void AIFCharacter::RotateToCamera()
{
	GetCharacterMovement()->bOrientRotationToMovement     = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

// needs the parameters to bind the delegate OnMontageStarted 
void AIFCharacter::RotateToCameraMontage(UAnimMontage* Montage)
{
	if (!AnimInstance->IsDrawOrSheatheMontage())
		bUseControllerRotationYaw = true;
}

void AIFCharacter::RotateDefault()
{
	GetCharacterMovement()->bOrientRotationToMovement     = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

// needs the parameters to bind the delegate OnMontageEnded
void AIFCharacter::RotateDefaultMontage(UAnimMontage* Montage, bool bInterrupted)
{
	bUseControllerRotationYaw = false;
	if (GetVelocity().Size() == 0)
	{
		GetCharacterMovement()->bOrientRotationToMovement     = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
}

void AIFCharacter::RecallAxe()
{
	FGameplayTagContainer AxeTemp;
	AxeTemp.AddTag(AXE_FLYING);
	AxeTemp.AddTag(AXE_LODGED);

	if (!Axe->HasMatchingGameplayTag(AXE_RETURNING) && (Axe->HasAnyMatchingGameplayTags(AxeTemp)))
	{
		AnimInstance->StopAllMontages(1);
		AnimInstance->AnimState.AddTag(ANIM_RECALL);
		AnimInstance->SetCanDoNextAction(false);
		Axe->Recall();
	}
}

void AIFCharacter::CatchAxe()
{
	AnimInstance->AnimState.RemoveTag(ANIM_RECALL);
	AnimInstance->AnimState.AddTag(ANIM_AXEHOLDING);
	AnimInstance->AnimState.AddTag(ANIM_DRAW);
    AnimInstance->SetCanDoNextAction(true);
    FName WeaponSocket(TEXT("Weapon_R"));
    Axe->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
	SetCameraShake();
}
