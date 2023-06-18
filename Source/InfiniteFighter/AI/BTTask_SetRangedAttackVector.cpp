// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetRangedAttackVector.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetRangedAttackVector::UBTTask_SetRangedAttackVector()
{
    NodeName = TEXT("SetVector");
}

EBTNodeResult::Type UBTTask_SetRangedAttackVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector Direction = (ControllingPawn->GetActorLocation() - TargetPawn->GetActorLocation()).GetSafeNormal();
	float DesiredDistance = 600.0f;
	FVector DesiredLocation = TargetPawn->GetActorLocation() + (Direction * DesiredDistance);

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("RangeAttackVector"), DesiredLocation);

    return EBTNodeResult::Succeeded;
}
