// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Components/TimelineComponent.h"
#include "IFEnemy.generated.h"

DECLARE_DELEGATE_OneParam(FOnHpChangedDelegate, float)

UCLASS()
class INFINITEFIGHTER_API AIFEnemy : public ACharacter, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIFEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Attack();
	
	void SetDead(float Time);

	UFUNCTION()
	void PlayExecution(UAnimMontage* AnimMontage);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> WarpPoint;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	FORCEINLINE void SetCanBeAttackedTrue() { bCanBeAttacked = true; }

	void ActivateStun();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UIFEnemyAnimInstance> AnimInstance;

	UFUNCTION()
	void SetEnemy(float InMaxHp, float InAttackDamage);

	UFUNCTION()
	void SetUI(UUserWidget* InUserWidget);
public:
	// GameplayTag
	UPROPERTY(BlueprintReadOnly, Category = GameplayTags)
	FGameplayTagContainer EnemyState;

	FOnHpChangedDelegate OnHpChanged;

protected:
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = EnemyState; };

private:
	FGameplayTag StunTag;

private:
	UFUNCTION()
	void UpdateDissolve(float InTimeline);

	UFUNCTION()
	void SetDestroy();

	UFUNCTION()
	void SetCurrentHp(float InCurrentHp);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Weapon;

	bool bCanBeAttacked;

	TObjectPtr<class AIFCharacter> PlayerCharacter;

	FTimerHandle StunTimer;

	FTimerHandle HitStopTimer;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> BloodParticle;

	TObjectPtr<class UMaterialInstanceDynamic> MIDCharacter;
	TObjectPtr<class UMaterialInstanceDynamic> MIDWeapon;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> DissolveTimeline;

	UPROPERTY()
	TObjectPtr<UCurveFloat> DissolveCurveFloat;

	FOnTimelineFloat OnDissolveTimelineFunction;

	FOnTimelineEvent OnDissolveTimelineFinished;

	UPROPERTY(VisibleAnywhere)
	float MaxHp;

	UPROPERTY(VisibleAnywhere)
	float CurrentHp;

	UPROPERTY(VisibleAnywhere)
	float AttackDamage;

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (AllowPrivateAccess))
	TObjectPtr<class UWidgetComponent> ExecutionWidget;

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (AllowPrivateAccess))
	TObjectPtr<class UWidgetComponent> HpBarWidget;
};
