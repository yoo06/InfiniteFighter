// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ShortAttack.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UBTTask_ShortAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ShortAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
