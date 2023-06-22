// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetPatrolPos.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackBoardComponent.h"

UBTTask_SetPatrolPos::UBTTask_SetPatrolPos()
{
}

EBTNodeResult::Type UBTTask_SetPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (ControllingPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
    if (NavSystem == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
    if (nullptr == TargetPawn)
    {
        return EBTNodeResult::Failed;
    }

    FVector Origin = TargetPawn->GetActorLocation();
    float PatrolRadious = 800.0f;
    FNavLocation NextPatrolPos;

    if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadious, NextPatrolPos))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPos"), NextPatrolPos.Location);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
