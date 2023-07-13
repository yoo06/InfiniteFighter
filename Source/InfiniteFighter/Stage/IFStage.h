// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IFStageTable.h"
#include "IFStage.generated.h"

UCLASS()
class INFINITEFIGHTER_API AIFStage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIFStage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	FORCEINLINE FIFStageTable GetStageTable(int32 InLevel) const { return StageTable.IsValidIndex(InLevel - 1) ? StageTable[InLevel - 1] : FIFStageTable(); }

	FORCEINLINE int32 GetCurrentLevel() const { return CurrentLevel; }

	UPROPERTY()
	int32 MaxLevel;

	UFUNCTION()
	void SetReward(AActor* DestroyedActor);

	UFUNCTION()
	void SetStage();

	UFUNCTION()
	void StartGame();
private:
	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USceneComponent> SceneComponent;

	TArray<FIFStageTable> StageTable;

	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = true))
	int32 CurrentLevel;

	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = true))
	int32 SpawnNumber;

	UPROPERTY(VisibleAnywhere, Category = Stage)
	TObjectPtr<class UBoxComponent> StageTriggerBox;

	UPROPERTY(VisibleAnywhere, Category = Stage)
	TObjectPtr<class UBoxComponent> StageBlockerBox;

	UPROPERTY(VisibleAnywhere, Category = UI)
	TObjectPtr<class UWidgetComponent> InteractionWidget;

	bool bCanGameStart;
	UFUNCTION()
	void OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnTriggerBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
