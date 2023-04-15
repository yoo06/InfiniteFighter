// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "IFAxe.generated.h"

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

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<UTimelineComponent> AxeGravityTimeline;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<UCurveFloat> AxeGravityCurveFloat;

	FOnTimelineFloat OnGravityTimelineFunction;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<UTimelineComponent> AxeRotateTimeline;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<UCurveFloat> AxeRotateCurveFloat;

	FOnTimelineFloat OnRotateTimelineFunction;

	UFUNCTION()
	void UpdateAxeGravity(float InGravity);

	UFUNCTION()
	void UpdateRotateGravity(float InRotate);

	void LodgePosition(const FHitResult& InHit);
};
