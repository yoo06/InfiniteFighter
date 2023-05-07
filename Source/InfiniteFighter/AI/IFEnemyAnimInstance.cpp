// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IFEnemyAnimInstance.h"

UIFEnemyAnimInstance::UIFEnemyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EXECUTE_VICTIM_MONTAGE
	(TEXT("/Game/InFiniteFighter/Characters/Animation/MatchedCombat/LegSweepStomp_Vic_Montage.LegSweepStomp_Vic_Montage"));
	if (EXECUTE_VICTIM_MONTAGE.Succeeded())
		ExecuteVictimMontage = EXECUTE_VICTIM_MONTAGE.Object;
}

void UIFEnemyAnimInstance::PlayExecuteVictimMontage()
{
	Montage_Play(ExecuteVictimMontage);
}

