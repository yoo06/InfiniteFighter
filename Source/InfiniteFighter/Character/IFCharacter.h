// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Interface/IFApplyItemInterface.h"
#include "IFCharacter.generated.h"

DECLARE_DELEGATE(FOnExecuteDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnExecutionEndDelegate);

UCLASS()
class INFINITEFIGHTER_API AIFCharacter : public ACharacter, public IGameplayTagAssetInterface, public IIFApplyItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIFCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void SetCameraShake();

	FORCEINLINE const class UCameraComponent* GetCamera() const { return Camera; };
	
	UFUNCTION()
	AActor* GetAxe();

	UPROPERTY()
	TObjectPtr<class AIFEnemy> Target;

	FOnExecuteDelegate OnExecute;

	FOnAttackEndDelegate OnAttackEnd;

	FOnExecutionEndDelegate OnExecutionEnd;

	FGameplayTagContainer CharacterState;

	UFUNCTION()
	FORCEINLINE float GetAttackDamage() { return AttackDamage; };

	UFUNCTION()
	void SetCurrentHp(float InCurrentHp);

	UFUNCTION()
	virtual void ApplyItem(class UIFItemData* InItemData) override;

protected:
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = CharacterState; };

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AIFPlayerController> PlayerController;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultContext;

	UPROPERTY(VisibleAnywhere)
	float MaxHp;

	UPROPERTY(VisibleAnywhere)
	float CurrentHp;

	UPROPERTY(VisibleAnywhere)
	float AttackDamage;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> DrawSheatheAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> BlockAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> WeakAttackAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> StrongAttackAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> AimAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ThrowAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> RecallAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> EvadeAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ExecuteAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> InteractionAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> StartAction;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class ACineCameraActor> CineCamera;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UTimelineComponent> AimTimeline;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCurveFloat> AimCurveFloat;

	FOnTimelineFloat OnAimTimelineFunction;
	
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<class AIFAxe> Axe;

	UPROPERTY()
	TObjectPtr<class UIFCharacterAnimInstance> AnimInstance;

	UPROPERTY()
	TObjectPtr<class UCommonInputSubsystem> InputSubsystem;

	UPROPERTY()
	FVector2D MovementVector;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class UExecutionAssetData>> ExecutionArray;

	FGameplayTag IdleState;

	FGameplayTag SprintState;

	FGameplayTag ParryingState;

	FGameplayTag BlockingState;

	FGameplayTag DamagedState;

	FGameplayTag AimState;

	FTimerHandle DamageTimer;

	FTimerHandle SlowTimer;

	UPROPERTY()
	TObjectPtr<class UParticleSystem> ParryingParticle;

	UPROPERTY()
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> WarpCollision;

	/* Give the Character Movement */
	void Move(const FInputActionValue& Value);

	/* Give the Camera Rotation */
	void Look(const FInputActionValue& Value);

	/* Let the Character Sprint while holding */
	void SprintStart();

	/* Max speed comes to default */
	void SprintEnd();

	/* Function for draw and sheathe motion */
	void DrawSheathe();

	/* Function to parry enemy attack */
	void Parrying();

	/* Blocking while holding */
	void BlockStart();

	/* Turns to Default */
	void BlockEnd();

	/* Attacks weak Attack */
	void WeakAttack();

	void StrongAttack();

	void AimStart();

	void AimEnd();

	void Evade();

	void Execute();

	void Interaction();

	void StartMenu();

	void SetDead();

	UFUNCTION()
	void UpdateAimCamera(float NewArmLength);

	void Throw();

	/* Draws weapon binded with delegate */
	UFUNCTION()
	void Draw();

	/* Sheathes weapon binded with delegate */
	UFUNCTION()
	void Sheathe();

	UFUNCTION()
	void RotateToCamera();

	UFUNCTION()
	void RotateToCameraMontage(UAnimMontage* Montage);

	UFUNCTION()
	void RotateDefault();

	UFUNCTION()
	void RotateDefaultMontage(UAnimMontage* Montage, bool bInterrupted);

	void RecallAxe();

	UFUNCTION()
	void CatchAxe();
};
