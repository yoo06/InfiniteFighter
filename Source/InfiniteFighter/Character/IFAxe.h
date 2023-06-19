// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "IFAxe.generated.h"

DECLARE_DELEGATE(FOnAxeCatch);

UCLASS()
class INFINITEFIGHTER_API AIFAxe : public AActor, public IGameplayTagAssetInterface
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

	FOnAxeCatch OnAxeCatch;

	FORCEINLINE void SetAxeState(FGameplayTag InGameplayTag) { AxeState.Reset(); AxeState.AddTag(InGameplayTag); }

protected:
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = AxeState; };


private:
	UPROPERTY(BlueprintReadWrite, Category = GameplayTags, meta = (AllowPrivateAccess))
	FGameplayTagContainer AxeState;

	FGameplayTag IdleTag;
	FGameplayTag FlyingTag;
	FGameplayTag LodgedTag;
	FGameplayTag ReturningTag;

private:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<USceneComponent> Pivot;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<USceneComponent> Lodge;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<USkeletalMeshComponent> Axe;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY()
	TObjectPtr<class AIFCharacter> Character;

	FVector CameraLocation;

	FRotator CameraRotation;

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

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystemComponent> TrailParticleComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystemComponent> CatchParticleComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> SparkParticle;

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

	FCollisionQueryParams Params;

	FVector  ReturnRightVector;
	FVector  ReturnLocation;
	FVector  ReturnStartLocation;
	FRotator ReturnStartRotation;
	FRotator ReturnStartCameraRotation;
	FRotator TiltingRotation;
	float	 DistanceFromCharacter;
	int      SpinCount;
};
