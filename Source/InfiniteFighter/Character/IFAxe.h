// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "IFAxe.generated.h"

DECLARE_DELEGATE(FOnAxeCatch);

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

	FOnAxeCatch OnAxeCatch;
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

	FVector CameraLocation;

	FRotator CameraRotation;

	EAxeState CurrentAxeState;

	/* Timeline for AxeGravity when thrown */ 
	UPROPERTY()
	TObjectPtr<UTimelineComponent> AxeGravityTimeline;

	UPROPERTY()
	TObjectPtr<UCurveFloat> AxeGravityCurveFloat;

	FOnTimelineFloat OnGravityTimelineFunction;

	/* Timeline for Axe Rotation*/ 
	UPROPERTY()
	TObjectPtr<UTimelineComponent> AxeRotateTimeline;

	UPROPERTY()
	TObjectPtr<UCurveFloat> AxeRotateCurveFloat;

	FOnTimelineFloat OnRotateTimelineFunction;

	FOnTimelineEvent OnRotateTimelineFinished;

	/* Timeline for wiggling before recalling */
	UPROPERTY()
	TObjectPtr<UTimelineComponent> WiggleTimeline;

	UPROPERTY()
	TObjectPtr<UCurveFloat> WiggleCurveFloat;

	FOnTimelineFloat OnWiggleTimelineFunction;

	FOnTimelineEvent OnWiggleTimelineFinished;

	/* Timeline for the return Function */
	UPROPERTY()
	TObjectPtr<UTimelineComponent> ReturnTiltStartTimeline;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> ReturnTiltEndTimeline;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> ReturnSpeedTimeline;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> RightVectorTimeline;

	UPROPERTY()
	TObjectPtr<UCurveFloat> ReturnTiltStartCurveFloat;

	UPROPERTY()
	TObjectPtr<UCurveFloat> ReturnSpeedCurveFloat;

	UPROPERTY()
	TObjectPtr<UCurveFloat> RightVectorCurveFloat;

	UPROPERTY()
	TObjectPtr<UCurveFloat> ReturnTiltEndCurveFloat;

	FOnTimelineFloat OnReturnTiltStartTimelineFunction;

	FOnTimelineFloat OnReturnTiltEndTimelineFunction;

	FOnTimelineFloat OnReturnSpeedTimelineFunction;

	FOnTimelineFloat OnRightVectorTimelineFunction;

	FOnTimelineEvent OnReturnTimelineFinished;


	/* Function for Updating Axe Projectile Gravity */
	UFUNCTION()
	void UpdateAxeGravity(float InGravity);

	/* Function for Updating Axe Rotation when thrown */
	UFUNCTION()
	void UpdateRotate(float InRotate);

	/* Adjusting the position of Axe when it is launched */
	void LodgePosition(const FHitResult& InHit);

	UFUNCTION()
	void UpdateWiggle(float InWigglePosition);

	UFUNCTION()
	void RecallMovement();

	UFUNCTION()
	void UpdateRightVector(float InVector);

	UFUNCTION()
	void UpdateReturnLocation(float InSpeed);

	UFUNCTION()
	void UpdateTiltStart(float InValue);

	UFUNCTION()
	void UpdateTiltEnd(float InValue);

	UFUNCTION()
	void CatchAxe();

	/* Calculates how many times axe should rotate */
	void CalculateSpin(float InTimelinePlayRate);

	UFUNCTION()
	void SpinStop();

	FVector  ReturnRightVector;
	FVector  ReturnLocation;
	FVector  ReturnStartLocation;
	FRotator ReturnStartRotation;
	FRotator ReturnStartCameraRotation;
	FRotator TiltingRotation;
	float	 DistanceFromCharacter;
	int      SpinCount;
};
