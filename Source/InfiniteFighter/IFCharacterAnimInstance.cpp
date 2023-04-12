// Fill out your copyright notice in the Description page of Project Settings.


#include "IFCharacterAnimInstance.h"
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
	AttackCombo		   = 0;

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
}

void UIFCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OnMontageStarted.AddDynamic(this, &UIFCharacterAnimInstance::DrawStateStart);
	OnMontageStarted.AddDynamic(this, &UIFCharacterAnimInstance::AnimNotify_CanDoNextActionFalse);
	OnMontageEnded.  AddDynamic(this, &UIFCharacterAnimInstance::DrawStateEnd);
	OnMontageEnded.  AddDynamic(this, &UIFCharacterAnimInstance::AttackStateEnd);

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
	if (!bIsBlockState)
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
		AttackCombo++;
		if (bIsAxeHolding)
		{
			if (!bIsAttackPlaying)
			{
				Montage_Play(WeaponWeakAttackMontage, 1.2f);
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

bool UIFCharacterAnimInstance::IsDrawOrSheatheMontage()
{
	return (Montage_IsActive(DrawMontage) || Montage_IsActive(SheatheMontage));
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
		AttackCombo = 0;
		bIsAttackPlaying = false;
	}

}

void UIFCharacterAnimInstance::AnimNotify_CanDoNextActionFalse(UAnimMontage* Montage)
{
	bCanDoNextAction = false;
}

void UIFCharacterAnimInstance::AnimNotify_CanDoNextActionTrue()
{
	bCanDoNextAction = true;
}

void UIFCharacterAnimInstance::AnimNotify_SheatheWeapon()
{
	OnSheathe.Execute();
}

void UIFCharacterAnimInstance::AnimNotify_DrawWeapon()
{
	OnDraw.Execute();
}

void UIFCharacterAnimInstance::AnimNotify_RotateCharacter()
{
	OnCharacterMove.Execute();
}

void UIFCharacterAnimInstance::AnimNotify_RotationDefault()
{
	OnCharacterStop.Execute();
}

void UIFCharacterAnimInstance::AnimNotify_ThrowPoint()
{
	bIsDrawState     = false;
	bIsAxeHolding    = false;
	bCanDoNextAction = true;

	OnThrow.Execute();
}

const FName UIFCharacterAnimInstance::GetAttackMontageSection(const int32& Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}