// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "IFEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API AIFEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AIFEnemyController();

	void RunAI();

	void StopAI();

	void SetTarget(APawn* Target);
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	class UBlackboardComponent* BlackBoardPtr;

};
