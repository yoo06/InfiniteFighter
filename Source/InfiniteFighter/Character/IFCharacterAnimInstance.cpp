// Fill out your copyright notice in the Description page of Project Settings.


#include "IFCharacterAnimInstance.h"
#include "IFCharacter.h"
#include "KismetAnimationLibrary.h"

UIFCharacterAnimInstance::UIFCharacterAnimInstance()
{
	CharacterDirection = 0.0f;
	CharacterSpeed	   = 0.0f;
	bIsAxeHolding      = false;
	bCanDoNextAction   = true;
	bIsDrawState       = false;
	bIsAttackPlaying   = false;
	bIsAimState		   = false;
	bIsRecalling	   = false;
	AttackCombo		   = 0;
	bIsDodging		   = false;

	// setting montages
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DRAW_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Combat/Draw_Montage.Draw_Montage"));
	if (DRAW_MONTAGE.Succeeded())
		DrawMontage = DRAW_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SHEATHE_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Combat/Sheathe_Montage.Sheathe_Montage"));
	if (SHEATHE_MONTAGE.Succeeded())
		SheatheMontage = SHEATHE_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PARRYING_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Combat/Parrying_Montage.Parrying_Montage"));
	if (PARRYING_MONTAGE.Succeeded())
		ParryingMontage = PARRYING_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> UNARM_WEAK_ATTACK_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Combat/UnArm_WeakAttack_Montage.UnArm_WeakAttack_Montage"));
	if (UNARM_WEAK_ATTACK_MONTAGE.Succeeded())
		UnArmWeakAttackMontage = UNARM_WEAK_ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> UNARM_STRONG_ATTACK_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Combat/UnArm_StrongAttack_Montage.UnArm_StrongAttack_Montage"));
	if (UNARM_STRONG_ATTACK_MONTAGE.Succeeded())
		UnArmStrongAttackMontage = UNARM_STRONG_ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> WEAPON_WEAK_ATTACK_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Combat/Weapon_WeakAttack_Montage.Weapon_WeakAttack_Montage"));
	if (WEAPON_WEAK_ATTACK_MONTAGE.Succeeded())
		WeaponWeakAttackMontage = WEAPON_WEAK_ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> THROW_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Combat/Throw_Montage.Throw_Montage"));
	if (THROW_MONTAGE.Succeeded())
		ThrowMontage = THROW_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FRONT_REACT_MONTAGE
	(TEXT("/Game/InFiniteFighter/AI/Animation/React/hit_body_front_Montage.hit_body_front_Montage"));
	if (FRONT_REACT_MONTAGE.Succeeded())
		ReactFrontMontage = FRONT_REACT_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BACK_REACT_MONTAGE
	(TEXT("/Game/InFiniteFighter/AI/Animation/React/hit_body_back_Montage.hit_body_back_Montage"));
	if (BACK_REACT_MONTAGE.Succeeded())
		ReactBackMontage = BACK_REACT_MONTAGE.Object;


	// Dodge Montages
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_BACK_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/DodgeBackward_Root_Montage.DodgeBackward_Root_Montage"));
	if (DODGE_BACK_MONTAGE.Succeeded())
		DodgeBackMontage = DODGE_BACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_BACK_LEFT_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/DodgeBackwardLeft_Root_Montage.DodgeBackwardLeft_Root_Montage"));
	if (DODGE_BACK_LEFT_MONTAGE.Succeeded())
		DodgeBackLeftMontage = DODGE_BACK_LEFT_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_BACK_RIGHT_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/DodgeBackwardRight_Root_Montage.DodgeBackwardRight_Root_Montage"));
	if (DODGE_BACK_RIGHT_MONTAGE.Succeeded())
		DodgeBackRightMontage = DODGE_BACK_RIGHT_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_FORWARD_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/DodgeForward_Root_Montage.DodgeForward_Root_Montage"));
	if (DODGE_FORWARD_MONTAGE.Succeeded())
		DodgeForwardMontage = DODGE_FORWARD_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_FORWARD_LEFT_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/DodgeForwardLeft_Root_Montage.DodgeForwardLeft_Root_Montage"));
	if (DODGE_FORWARD_LEFT_MONTAGE.Succeeded())
		DodgeForwardLeftMontage = DODGE_FORWARD_LEFT_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_FORWARD_RIGHT_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/DodgeForwardRight_Root_Montage.DodgeForwardRight_Root_Montage"));
	if (DODGE_FORWARD_RIGHT_MONTAGE.Succeeded())
		DodgeForwardRightMontage = DODGE_FORWARD_RIGHT_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_LEFT_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/DodgeLeft_Root_Montage.DodgeLeft_Root_Montage"));
	if (DODGE_LEFT_MONTAGE.Succeeded())
		DodgeLeftMontage = DODGE_LEFT_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_RIGHT_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/DodgeRight_Root_Montage.DodgeRight_Root_Montage"));
	if (DODGE_RIGHT_MONTAGE.Succeeded())
		DodgeRightMontage = DODGE_RIGHT_MONTAGE.Object;

	// Roll Montages
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_BACK_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/RollBackward_Root_Montage.RollBackward_Root_Montage"));
	if (ROLL_BACK_MONTAGE.Succeeded())
		RollBackMontage = ROLL_BACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_BACK_LEFT_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/RollBackwardLeft_Root_Montage.RollBackwardLeft_Root_Montage"));
	if (ROLL_BACK_LEFT_MONTAGE.Succeeded())
		RollBackLeftMontage = ROLL_BACK_LEFT_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_BACK_RIGHT_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/RollBackwardRight_Root_Montage.RollBackwardRight_Root_Montage"));
	if (ROLL_BACK_RIGHT_MONTAGE.Succeeded())
		RollBackRightMontage = ROLL_BACK_RIGHT_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_FORWARD_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/RollForward_Root_Montage.RollForward_Root_Montage"));
	if (ROLL_FORWARD_MONTAGE.Succeeded())
		RollForwardMontage = ROLL_FORWARD_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_FORWARD_LEFT_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/RollForwardLeft_Root_Montage.RollForwardLeft_Root_Montage"));
	if (ROLL_FORWARD_LEFT_MONTAGE.Succeeded())
		RollForwardLeftMontage = ROLL_FORWARD_LEFT_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_FORWARD_RIGHT_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/RollForwardRight_Root_Montage.RollForwardRight_Root_Montage"));
	if (ROLL_FORWARD_RIGHT_MONTAGE.Succeeded())
		RollForwardRightMontage = ROLL_FORWARD_RIGHT_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_LEFT_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/RollLeft_Root_Montage.RollLeft_Root_Montage"));
	if (ROLL_LEFT_MONTAGE.Succeeded())
		RollLeftMontage = ROLL_LEFT_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_RIGHT_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/Evade/RollRight_Root_Montage.RollRight_Root_Montage"));
	if (ROLL_RIGHT_MONTAGE.Succeeded())
		RollRightMontage = ROLL_RIGHT_MONTAGE.Object;
}

void UIFCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OnMontageStarted.AddDynamic(this, &UIFCharacterAnimInstance::DrawStateStart);
	OnMontageStarted.AddDynamic(this, &UIFCharacterAnimInstance::CanDoNextActionFalse);
	OnMontageEnded.  AddDynamic(this, &UIFCharacterAnimInstance::DrawStateEnd);
	OnMontageEnded.  AddDynamic(this, &UIFCharacterAnimInstance::AttackStateEnd);

	DodgeMontages.Add(DodgeBackMontage);
	DodgeMontages.Add(DodgeBackLeftMontage);
	DodgeMontages.Add(DodgeBackRightMontage);
	DodgeMontages.Add(DodgeForwardMontage);
	DodgeMontages.Add(DodgeForwardLeftMontage);
	DodgeMontages.Add(DodgeForwardRightMontage);
	DodgeMontages.Add(DodgeLeftMontage);
	DodgeMontages.Add(DodgeRightMontage);

}

void UIFCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();

	if (::IsValid(Pawn))
	{
		// get speed
		CharacterSpeed = Pawn->GetVelocity().Size();

		// get direction
		CharacterDirection = UKismetAnimationLibrary::CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
	}
}

// play sheathe if equiped, play draw if unequiped
void UIFCharacterAnimInstance::PlayDrawSheatheMontage()
{
	if (!bIsBlockState && !bIsAimState)
	{
		if (bIsAxeHolding)
		{
			if (bCanDoNextAction)
			{
				Montage_Play(SheatheMontage);
				bIsAxeHolding = false;
			}
		}
		else
		{
			if (bCanDoNextAction)
			{
				Montage_Play(DrawMontage);
				bIsAxeHolding = true;
			}
		}
	}
}

void UIFCharacterAnimInstance::PlayParryingMontage()
{
	if(bCanDoNextAction)
		Montage_Play(ParryingMontage);
}

void UIFCharacterAnimInstance::PlayWeakAttackMontage()
{
	if (bCanDoNextAction && !bIsAimState)
	{
		AIFCharacter* Character = CastChecked<AIFCharacter>(TryGetPawnOwner());
		Character->OnAttackEnd.Broadcast();

		AttackCombo = FMath::Clamp(AttackCombo+1, AttackCombo, 3);
		if (bIsAxeHolding)
		{
			if (!bIsAttackPlaying)
			{
				Montage_Play(WeaponWeakAttackMontage, 1.3f);
				bIsAttackPlaying = true;
			}
			else
			{
				Montage_JumpToSection(GetAttackMontageSection(AttackCombo));
			}
		}
		else
		{
			if (!bIsAttackPlaying)
			{
				Montage_Play(UnArmWeakAttackMontage);
				bIsAttackPlaying = true;
			}
			else
			{
				Montage_JumpToSection(GetAttackMontageSection(AttackCombo));
			}
		}

	}
}

void UIFCharacterAnimInstance::PlayStrongAttackMontage()
{
	if (bCanDoNextAction)
	{
		AIFCharacter* Character = CastChecked<AIFCharacter>(TryGetPawnOwner());
		Character->OnAttackEnd.Broadcast();

		if (bIsAxeHolding)
		{
			// Montage_Play(WeaponStrongAttackMontage);
		}
		else
		{
			Montage_Play(UnArmStrongAttackMontage);
		}

	}
}

void UIFCharacterAnimInstance::PlayThrowMontage()
{
	if(bCanDoNextAction && bIsAxeHolding)
		Montage_Play(ThrowMontage);
}

void UIFCharacterAnimInstance::PlayDodgeMontage(FVector2D Direction)
{
    if (bCanDoNextAction)
    {
        for (const auto& CurrentMontage : DodgeMontages)
        {
            if (CurrentMontage == GetCurrentActiveMontage())
            {
				if (Direction.X >= -0.5 && Direction.X < 0.5 && Direction.Y > 0.5)
					Montage_Play(RollForwardMontage);
				else if (Direction.X >= 0.5 && Direction.Y >= 0.5)
					Montage_Play(RollForwardRightMontage);
				else if (Direction.X > 0.5 && Direction.Y < 0.5 && Direction.Y >= -0.5)
					Montage_Play(RollRightMontage);
				else if (Direction.X > 0.5 && Direction.Y < -0.5)
					Montage_Play(RollBackRightMontage);
				else if (Direction.X < -0.5 && Direction.Y > 0.5)
					Montage_Play(RollForwardLeftMontage);
				else if (Direction.X < -0.5 && Direction.Y < 0.5 && Direction.Y >= -0.5)
					Montage_Play(RollLeftMontage);
				else if (Direction.X <= -0.5 && Direction.Y <= -0.5)
					Montage_Play(RollBackLeftMontage);
				else if (Direction.Y < -0.5 && Direction.X >= -0.5 && Direction.X < 0.5)
					Montage_Play(RollBackMontage);
				else
					return;

                return;
            }
        }
		if (Direction.X >= -0.5 && Direction.X < 0.5 && Direction.Y > 0.5)
			Montage_Play(DodgeForwardMontage);
		else if (Direction.X >= 0.5 && Direction.Y >= 0.5)
			Montage_Play(DodgeForwardRightMontage);
		else if (Direction.X > 0.5 && Direction.Y < 0.5 && Direction.Y >= -0.5)
			Montage_Play(DodgeRightMontage);
		else if (Direction.X > 0.5 && Direction.Y < -0.5)
			Montage_Play(DodgeBackRightMontage);
		else if (Direction.X < -0.5 && Direction.Y > 0.5)
			Montage_Play(DodgeForwardLeftMontage);
		else if (Direction.X < -0.5 && Direction.Y < 0.5 && Direction.Y >= -0.5)
			Montage_Play(DodgeLeftMontage);
		else if (Direction.X <= -0.5 && Direction.Y <= -0.5)
			Montage_Play(DodgeBackLeftMontage);
		else if (Direction.Y < -0.5 && Direction.X >= -0.5 && Direction.X < 0.5)
			Montage_Play(DodgeBackMontage);
		else
			return;
    }
}


bool UIFCharacterAnimInstance::IsDrawOrSheatheMontage()
{
	return (Montage_IsActive(DrawMontage) || Montage_IsActive(SheatheMontage));
}

void UIFCharacterAnimInstance::React(AActor* Target, AActor* Causer)
{
	// check if character and enemy are facing(DotProduct on both character's forward vector)
	float DotProduct = FVector::DotProduct(Causer->GetActorForwardVector(), Target->GetActorForwardVector());

	FRotator CurrentRotation = Target->GetActorRotation();

	if (DotProduct < 0) Montage_Play(ReactFrontMontage);
	else				Montage_Play(ReactBackMontage);
}


void UIFCharacterAnimInstance::DrawStateStart(UAnimMontage* Montage)
{
	if (Montage == SheatheMontage)
		bIsDrawState = false;
}

void UIFCharacterAnimInstance::DrawStateEnd(UAnimMontage* Montage, bool bInterrupted)
{
	// Only Set true when the montage is fully played
	if (Montage == DrawMontage && !bInterrupted)
		bIsDrawState = true;
}

void UIFCharacterAnimInstance::AttackStateEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == UnArmWeakAttackMontage || Montage == WeaponWeakAttackMontage)
	{
		bIsAttackPlaying = false;
		AttackCombo		 = 0;
		if (RootMotionMode == ERootMotionMode::IgnoreRootMotion)
			SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
	}
}

void UIFCharacterAnimInstance::AnimNotify_CanDoNextActionFalse()
{
	bCanDoNextAction = false;
}

void UIFCharacterAnimInstance::CanDoNextActionFalse(UAnimMontage* Montage)
{
	bCanDoNextAction = false;
}

void UIFCharacterAnimInstance::AnimNotify_CanDoNextActionTrue()
{
	bCanDoNextAction = true;
}

void UIFCharacterAnimInstance::AnimNotify_SheatheWeapon()
{
	OnSheathe.ExecuteIfBound();
}

void UIFCharacterAnimInstance::AnimNotify_DrawWeapon()
{
	OnDraw.ExecuteIfBound();
}

void UIFCharacterAnimInstance::AnimNotify_RotateCharacter()
{
	OnCharacterMove.ExecuteIfBound();
}

void UIFCharacterAnimInstance::AnimNotify_RotationDefault()
{
	OnCharacterStop.ExecuteIfBound();
}

void UIFCharacterAnimInstance::AnimNotify_ThrowPoint()
{
	bIsDrawState     = false;
	bIsAxeHolding    = false;
	bCanDoNextAction = true;

	OnThrow.ExecuteIfBound();
}

void UIFCharacterAnimInstance::AnimNotify_CatchEnd()
{
	OnCatchEnd.ExecuteIfBound();
}

void UIFCharacterAnimInstance::AnimNotify_EndParryingPoint()
{
	OnParryingEnd.ExecuteIfBound();
}

const FName UIFCharacterAnimInstance::GetAttackMontageSection(const int32& Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}