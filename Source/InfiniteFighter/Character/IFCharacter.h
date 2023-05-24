// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "IFCharacter.generated.h"

DECLARE_DELEGATE(FOnExecuteDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class INFINITEFIGHTER_API AIFCharacter : public ACharacter
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

	FORCEINLINE const class UCameraComponent* AIFCharacter::GetCamera() const { return Camera; };

	UPROPERTY()
	TObjectPtr<class AIFEnemy> Target;

	FOnExecuteDelegate OnExecute;

	FOnAttackEndDelegate OnAttackEnd;
private:
	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> DrawSheatheAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ParryingAction;

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

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class UCameraComponent> Camera;

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

	UPROPERTY(VisibleAnywhere, Category = UI)
	TObjectPtr<class UIFAimWidget> AimHUD;

	UPROPERTY()
	TSubclassOf<class UIFAimWidget> AimHUDClass;

	UPROPERTY()
	TObjectPtr<class UIFCharacterAnimInstance> AnimInstance;

	UPROPERTY()
	TObjectPtr<class UCommonInputSubsystem> InputSubsystem;

	UPROPERTY()
	FVector2D MovementVector;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class UExecutionAssetData>> ExecutionArray;

	UPROPERTY()
	bool bParryingPoint;

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
