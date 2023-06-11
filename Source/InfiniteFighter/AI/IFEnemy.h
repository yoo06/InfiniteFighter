// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IFEnemy.generated.h"

UCLASS()
class INFINITEFIGHTER_API AIFEnemy : public ACharacter
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
	
	void SetDead();

	UFUNCTION()
	void PlayMontage(UAnimMontage* AnimMontage);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> WarpPoint;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	FORCEINLINE void SetCanBeAttackedTrue() { bCanBeAttacked = true; }

	void ActivateStun();
	void DeactivateStun();
	FORCEINLINE bool GetStunState();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UIFEnemyAnimInstance> AnimInstance;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Weapon;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> WarpCollision;

	bool bCanBeAttacked;

	TObjectPtr<class AIFCharacter> PlayerCharacter;

	FTimerHandle StunTimer;

	FTimerHandle StiffTimer;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> BloodParticle;
};
