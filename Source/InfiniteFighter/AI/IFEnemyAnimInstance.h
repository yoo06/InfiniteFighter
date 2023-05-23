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

	void React(AActor* Target, AActor* Causer);
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ReactBackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ReactFrontMontage;
};
