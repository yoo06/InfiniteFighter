// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IFEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UIFEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UIFEnemyAnimInstance();

	void PlayExecuteVictimMontage();

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ExecuteVictimMontage;

};
