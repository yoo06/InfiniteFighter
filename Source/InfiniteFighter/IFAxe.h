// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "IFAxe.generated.h"

UENUM()
enum class EAxeState
{
	Idle,
	Flying,
	Lodged,
	Returning,
};

UCLASS()
class INFINITEFIGHTER_API AIFAxe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIFAxe();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	UFUNCTION()
	void Throw();

	UFUNCTION()
	void Recall();

	FORCEINLINE const EAxeState GetAxeState() const { return CurrentAxeState; }
	FORCEINLINE void SetAxeState(EAxeState InAxeState) { CurrentAxeState = InAxeState; }
private:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<USceneComponent> Pivot;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<USceneComponent> Lodge;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<UStaticMeshComponent> Axe;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY()
	TObjectPtr<class AIFCharacter> Character;

	FRotator CameraRotation;

	EAxeState CurrentAxeState;

	/* Timeline for AxeGravity when thrown */ 
	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<UTimelineComponent> AxeGravityTimeline;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<UCurveFloat> AxeGravityCurveFloat;

	FOnTimelineFloat OnGravityTimelineFunction;

	/* Timeline for AxeGravity when thrown*/ 
	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<UTimelineComponent> AxeRotateTimeline;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<UCurveFloat> AxeRotateCurveFloat;

	FOnTimelineFloat OnRotateTimelineFunction;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<UTimelineComponent> WiggleTimeline;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<UCurveFloat> WiggleCurveFloat;

	FOnTimelineFloat OnWiggleTimelineFunction;

	FOnTimelineEvent OnWiggleTimelineFinished;

	/* Function for Updating Axe Projectile Gravity */
	UFUNCTION()
	void UpdateAxeGravity(float InGravity);

	/* Function for Updating Axe Rotation when thrown */
	UFUNCTION()
	void UpdateRotateGravity(float InRotate);

	/* Adjusting the position of Axe when it is launched */
	void LodgePosition(const FHitResult& InHit);

	UFUNCTION()
	void UpdateWiggle(float InWigglePosition);

	UFUNCTION()
	void RecallMovement();
};
