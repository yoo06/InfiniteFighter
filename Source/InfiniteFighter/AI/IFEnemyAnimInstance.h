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

	FORCEINLINE void SetStunState(bool bInStunState) { bIsStunned = bInStunState; }
	FORCEINLINE bool GetStunState() const { return bIsStunned; }

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void React(AActor* Target, AActor* Causer);

	void PlayAttackMontage();
private:
	// Montage
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ReactBackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ReactFrontMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> AttackMontage;

	// variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float EnemyDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float EnemySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsStunned;
};
