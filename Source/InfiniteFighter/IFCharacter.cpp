// Fill out your copyright notice in the Description page of Project Settings.


#include "IFCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "CommonInputSubsystem.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IFCharacterAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "IFAxe.h"
#include "IFAimWidget.h"

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

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_PARRYING
	(TEXT("/Game/InFiniteFighter/Input/Actions/IA_Parrying.IA_Parrying"));
	if (IA_PARRYING.Succeeded())
		ParryingAction = IA_PARRYING.Object;

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

	// Setting properties for Aiming the Axe
	static ConstructorHelpers::FObjectFinder<UCurveFloat>AIM_CURVE_FLOAT
	(TEXT("/Game/InFiniteFighter/Miscellaneous/AimCurve.AimCurve"));
	if (AIM_CURVE_FLOAT.Succeeded())
		AimCurveFloat = AIM_CURVE_FLOAT.Object;

	static ConstructorHelpers::FClassFinder<UIFAimWidget>AIM_HUD_C
	(TEXT("/Game/InFiniteFighter/Characters/Widget/Aim_Hud.Aim_Hud_C"));
	if (AIM_HUD_C.Succeeded())
		AimHUDClass = AIM_HUD_C.Class;

	AimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AIM_TIMELINE"));

	// creating parts for character (springarm, camera)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING_ARM"));
	SpringArm->SetupAttachment(GetMesh(), "spine_03");

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	// positioning the skeletal mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	// setting spring arm
	SpringArm->ProbeSize                = 16.0f;
	SpringArm->TargetArmLength          = 170.0f;
	SpringArm->SocketOffset             = FVector(0.0f, 50.0f, 30.0f);
	SpringArm->bUsePawnControlRotation  = true;
	SpringArm->bInheritPitch		    = true;
	SpringArm->bInheritYaw			    = true;
	SpringArm->bInheritRoll			    = true;
	// enabling camera lag for a smoother look
	SpringArm->bEnableCameraLag		    = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagMaxDistance     = 20.0f;
	SpringArm->CameraLagSpeed           = 2.0f;
	SpringArm->CameraRotationLagSpeed   = 10.0f;
	bUseControllerRotationYaw           = false;

	GetCharacterMovement()->MaxWalkSpeed			   = 400.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 50.0f;
	GetCharacterMovement()->RotationRate			   = FRotator(0.0f, 360.0f, 0.0f);

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

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = -60.0f;
		PlayerCameraManager->ViewPitchMax =  60.0f;
	}

	RotateDefault();

	AimHUD = CreateWidget<UIFAimWidget>(GetWorld()->GetFirstPlayerController(), AimHUDClass);
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
		EnhancedInputComponent->BindAction(ParryingAction,     ETriggerEvent::Triggered, this, &AIFCharacter::Parrying);
		EnhancedInputComponent->BindAction(BlockAction,        ETriggerEvent::Triggered, this, &AIFCharacter::BlockStart);
		EnhancedInputComponent->BindAction(BlockAction,        ETriggerEvent::Completed, this, &AIFCharacter::BlockEnd);
		EnhancedInputComponent->BindAction(WeakAttackAction,   ETriggerEvent::Triggered, this, &AIFCharacter::WeakAttack);
		EnhancedInputComponent->BindAction(StrongAttackAction, ETriggerEvent::Triggered, this, &AIFCharacter::StrongAttack);
		EnhancedInputComponent->BindAction(AimAction,		   ETriggerEvent::Triggered, this, &AIFCharacter::AimStart);
		EnhancedInputComponent->BindAction(AimAction,		   ETriggerEvent::Completed, this, &AIFCharacter::AimEnd);
		EnhancedInputComponent->BindAction(ThrowAction,		   ETriggerEvent::Triggered, this, &AIFCharacter::Throw);
		EnhancedInputComponent->BindAction(RecallAction,	   ETriggerEvent::Triggered, this, &AIFCharacter::RecallAxe);
	}

}

void AIFCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Creating the Axe and Attaching it to Back
	FName BackSocket(TEXT("Weapon_Back"));
	Axe = GetWorld()->SpawnActor<AIFAxe>(FVector::ZeroVector, FRotator::ZeroRotator);

	if (nullptr != Axe)
		Axe->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, BackSocket);

	// cast IFCharacterAnimInstance to Character's AnimInstance
	AnimInstance = Cast<UIFCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	
	// Binding the Delegates
	AnimInstance->OnDraw.		   BindUObject(this, &AIFCharacter::Draw);
	AnimInstance->OnSheathe.	   BindUObject(this, &AIFCharacter::Sheathe);
	AnimInstance->OnCharacterMove. BindUObject(this, &AIFCharacter::RotateToCamera);
	AnimInstance->OnCharacterStop. BindUObject(this, &AIFCharacter::RotateDefault);
	AnimInstance->OnMontageStarted.AddDynamic (this, &AIFCharacter::RotateToCameraMontage);
	AnimInstance->OnMontageEnded.  AddDynamic (this, &AIFCharacter::RotateDefaultMontage);
	AnimInstance->OnThrow.		   BindUObject(Axe,  &AIFAxe::Throw);
	Axe			->OnAxeCatch.	   BindUObject(this, &AIFCharacter::CatchAxe);

	OnAimTimelineFunction.BindDynamic(this, &AIFCharacter::UpdateAimCamera);
	AimTimeline->AddInterpFloat(AimCurveFloat, OnAimTimelineFunction);
}

void AIFCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
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
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AIFCharacter::SprintStart()
{
	if (GetCharacterMovement()->MaxWalkSpeed != 600.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

void AIFCharacter::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void AIFCharacter::DrawSheathe()
{
	if(Axe->GetAxeState() == EAxeState::Idle)
		AnimInstance->PlayDrawSheatheMontage();
}

void AIFCharacter::Parrying()
{
	AnimInstance->PlayParryingMontage();
}

void AIFCharacter::BlockStart()
{
	if (GetCharacterMovement()->MaxWalkSpeed != 200.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed  = 200.0f;
		AnimInstance->SetBlockState(true);
	}
}

void AIFCharacter::BlockEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	AnimInstance->SetBlockState(false);
}

void AIFCharacter::WeakAttack()
{
	AnimInstance->PlayWeakAttackMontage();
}

void AIFCharacter::StrongAttack()
{
	AnimInstance->PlayStrongAttackMontage();
}

void AIFCharacter::AimStart()
{
	RotateToCamera();
	

	if (GetCharacterMovement()->MaxWalkSpeed != 200.0f)
	{
		AimHUD->AddToViewport();
		AnimInstance->SetAimState(true);
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		AimTimeline->Play();
		AimTimeline->SetPlayRate(1.4f);
	}

    FVector StartLocation = Camera->GetComponentLocation() + Camera->GetComponentRotation().Vector() * 100;
    FVector EndLocation = Camera->GetComponentLocation() + Camera->GetComponentRotation().Vector() * 3000;

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

	AnimInstance->SetAimState(false);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	AimTimeline->Reverse();
	AimTimeline->SetPlayRate(0.9f);
}

void AIFCharacter::UpdateAimCamera(float NewArmLength)
{
	SpringArm->TargetArmLength = NewArmLength;
}

void AIFCharacter::Throw()
{
	AnimInstance->StopAllMontages(1);
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
	if (!AnimInstance->GetRecall() && (Axe->GetAxeState() == EAxeState::Flying || Axe->GetAxeState() == EAxeState::Lodged))
	{
		AnimInstance->SetRecall(true);
		AnimInstance->SetCanDoNextAction(false);
		Axe->Recall();
	}
}

void AIFCharacter::CatchAxe()
{
		AnimInstance->SetRecall(false);
		AnimInstance->SetAxeHolding(true);
		AnimInstance->SetDrawState(true);
		AnimInstance->SetCanDoNextAction(true);
		FName WeaponSocket(TEXT("Weapon_R"));
		Axe->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
}
