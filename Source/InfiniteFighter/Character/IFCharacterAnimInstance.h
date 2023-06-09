// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "IFCharacterAnimInstance.generated.h"

DECLARE_DELEGATE(FOnAxeDrawDelegate)
DECLARE_DELEGATE(FOnAxeSheatheDelegate)
DECLARE_DELEGATE(FOnAxeThrowDelegate)
DECLARE_DELEGATE(FOnCharacterMoveDelegate)
DECLARE_DELEGATE(FOnCharacterStopDelegate)
DECLARE_DELEGATE(FOnCatchEndDelegate)
DECLARE_DELEGATE(FOnParryingEndDelegate)

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UIFCharacterAnimInstance : public UAnimInstance, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
public:
	UIFCharacterAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = AnimState; };

public:
	UPROPERTY(BlueprintReadWrite, Category = GameplayTags, meta = (AllowPrivateAccess))
	FGameplayTagContainer AnimState;

	FORCEINLINE void SetCanDoNextAction(const bool& bInNextAction) { bCanDoNextAction = bInNextAction; };

	/* Plays Draw and Sheathe Montage */
	void PlayDrawSheatheMontage();

	/* Plays Parrying Montage */
	void PlayParryingMontage();

	/* Plays Weak Attack Montage */
	void PlayWeakAttackMontage();

	/* Plays Strong Attack Montage */
	void PlayStrongAttackMontage();

	/* Plays Throwing Montage */
	void PlayThrowMontage();

	/* Plays Dodge Montage */
	void PlayDodgeMontage(FVector2D Direction);

	void PlayBackDownMontage();

	/* Checks the Montage if it is Playing Draw Montage or Sheathe Montage */
	bool IsDrawOrSheatheMontage();

	void React(AActor* Target, AActor* Causer);

	FOnAxeDrawDelegate OnDraw;

	FOnAxeSheatheDelegate OnSheathe;

	FOnAxeThrowDelegate OnThrow;

	FOnCharacterMoveDelegate OnCharacterMove;

	FOnCharacterStopDelegate OnCharacterStop;

	FOnCatchEndDelegate OnCatchEnd;

	FOnParryingEndDelegate OnParryingEnd;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class AIFCharacter> CharacterRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CharacterDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CharacterSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bCanDoNextAction;

	FGameplayTag AxeHoldingState;

	FGameplayTag DrawState;

	FGameplayTag BlockState;

	FGameplayTag AimState;

	FGameplayTag AttackState;

	FGameplayTag RecallState;

	UPROPERTY()
	int32 AttackCombo;

	UPROPERTY()
	TArray<UAnimMontage*> DodgeMontages;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DrawMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> SheatheMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ParryingMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> UnArmWeakAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> UnArmStrongAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> WeaponWeakAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ThrowMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ReactFrontMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ReactBackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> BackDownMontage;

	// 8direction Dodge Montage
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DodgeBackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DodgeBackLeftMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DodgeBackRightMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DodgeForwardMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DodgeForwardLeftMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DodgeForwardRightMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DodgeLeftMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DodgeRightMontage;


	// 8direction Roll Montage
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> RollBackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> RollBackLeftMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> RollBackRightMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> RollForwardMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> RollForwardLeftMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> RollForwardRightMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> RollLeftMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> RollRightMontage;

	/* Function to play when drawState starts */
	UFUNCTION()
	void DrawStateStart(UAnimMontage* Montage);

	/* Function when Draw State is ended */
	UFUNCTION()
	void DrawStateEnd(UAnimMontage* Montage, bool bInterrupted);

	/* Function when Attacking is finished */
	UFUNCTION()
	void AttackStateEnd(UAnimMontage* Montage, bool bInterrupted);

	/* Notify function to Make bCanDoNextAction to false */
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

	/* notify to Set bUseControllerDesiredRotation to true */
	UFUNCTION()
	void AnimNotify_RotateCharacter();

	/* notify to Set bUseControllerDesiredRotation to false */
	UFUNCTION()
	void AnimNotify_RotationDefault();

	/* notify for the timing of Axe throw */
	UFUNCTION()
	void AnimNotify_ThrowPoint();

	/* notify to set the axe position */
	UFUNCTION()
	void AnimNotify_CatchEnd();

	UFUNCTION()
	void AnimNotify_EndParryingPoint();

	/* returns the Montage Section Attack1~3 */
	const FName GetAttackMontageSection(const int32& Section);
};
