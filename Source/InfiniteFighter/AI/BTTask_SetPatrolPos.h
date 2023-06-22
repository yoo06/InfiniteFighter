// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UBTTask_SetPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
