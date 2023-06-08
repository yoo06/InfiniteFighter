// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ShortAttack.h"
#include "AIController.h"
#include "IFEnemy.h"

UBTTask_ShortAttack::UBTTask_ShortAttack()
{
}

EBTNodeResult::Type UBTTask_ShortAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	AIFEnemy* AIPawn = Cast<AIFEnemy>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	AIPawn->Attack();
	return EBTNodeResult::Succeeded;
}
