// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IFCharacterAnimInstance.generated.h"

DECLARE_DELEGATE(FOnAxeDrawDelegate);
DECLARE_DELEGATE(FOnAxeSheatheDelegate);
DECLARE_DELEGATE(FOnAxeThrowDelegate);
DECLARE_DELEGATE(FOnCharacterMoveDelegate);
DECLARE_DELEGATE(FOnCharacterStopDelegate);

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UIFCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UIFCharacterAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	FORCEINLINE void SetAimState(const bool& bInAimState) { bIsAimState = bInAimState; };

	FORCEINLINE void SetBlockState(const bool& bInBlockState) { bIsBlockState = bInBlockState; };

	FORCEINLINE const bool GetRecall() const { return bIsRecalling; };

	FORCEINLINE void SetRecall(const bool& bInRecall) { bIsRecalling = bInRecall; };

	/* Plays Draw and Sheathe Montage */
	void PlayDrawSheatheMontage();

	/* Plays Parrying Montage */
	void PlayParryingMontage();

	/* Plays Attack Montage */
	void PlayWeakAttackMontage();

	void PlayStrongAttackMontage();

	void PlayThrowMontage();

	bool IsDrawOrSheatheMontage();

	FOnAxeDrawDelegate OnDraw;

	FOnAxeSheatheDelegate OnSheathe;

	FOnAxeThrowDelegate OnThrow;

	FOnCharacterMoveDelegate OnCharacterMove;

	FOnCharacterStopDelegate OnCharacterStop;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CharacterDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CharacterSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsAxeHolding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bCanDoNextAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsDrawState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsBlockState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsAimState;

	UPROPERTY()
	bool bIsAttackPlaying;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsRecalling;

	UPROPERTY()
	int32 AttackCombo;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DrawMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SheatheMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ParryingMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* UnArmWeakAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* UnArmStrongAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* WeaponWeakAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ThrowMontage;

	/* Function to play when drawState starts */
	UFUNCTION()
	void DrawStateStart(UAnimMontage* Montage);

	/* Function when Draw State is ended */
	UFUNCTION()
	void DrawStateEnd(UAnimMontage* Montage, bool bInterrupted);

	/* Function when Attacking is finished */
	UFUNCTION()
	void AttackStateEnd(UAnimMontage* Montage, bool bInterrupted);

	/* Make bCanDoNextAction to false */
	UFUNCTION()
	void AnimNotify_CanDoNextActionFalse(UAnimMontage* Montage);
	
	/* Notify to make bCanDoNextAction to true */
	UFUNCTION()
	void AnimNotify_CanDoNextActionTrue();

	/* notify to execute the delegate OnSheathe */
	UFUNCTION()
	void AnimNotify_SheatheWeapon();

	/* notify to execute the delegate onDraw */
	UFUNCTION()
	void AnimNotify_DrawWeapon();

	UFUNCTION()
	void AnimNotify_RotateCharacter();

	UFUNCTION()
	void AnimNotify_RotationDefault();

	UFUNCTION()
	void AnimNotify_ThrowPoint();

	/* returns the Montage Section Attack1~3 */
	const FName GetAttackMontageSection(const int32& Section);
};
