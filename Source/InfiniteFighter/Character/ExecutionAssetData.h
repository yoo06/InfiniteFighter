// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ExecutionAssetData.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UExecutionAssetData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UExecutionAssetData();

	void CreateSequencePlayer();

	void Play();

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> VictimMontage;

	UPROPERTY(EditAnywhere, Category = Camera)
	TObjectPtr<class ULevelSequence> LevelSequence;

	UPROPERTY(EditAnywhere, Category = Camera)
	TObjectPtr<class ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY(EditAnywhere, Category = Camera)
	FVector WarpPoint;
};
