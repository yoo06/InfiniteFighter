// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IFEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


AIFEnemyController::AIFEnemyController()
{
    static ConstructorHelpers::FObjectFinder<UBlackboardData> BLACKBOARD
    (TEXT("/Game/InFiniteFighter/AI/AI/BB_IFEnemy.BB_IFEnemy"));
    if (BLACKBOARD.Succeeded())
        BBAsset = BLACKBOARD.Object;

    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BEHAVIORTREE
    (TEXT("/Game/InFiniteFighter/AI/AI/BT_IFEnemy.BT_IFEnemy"));
    if (BEHAVIORTREE.Succeeded())
        BTAsset = BEHAVIORTREE.Object;
}

void AIFEnemyController::RunAI()
{
    BlackBoardPtr = Blackboard.Get();
    if (UseBlackboard(BBAsset, BlackBoardPtr))
    {
        bool RunResult = RunBehaviorTree(BTAsset);
        ensure(RunResult);
    }
}

void AIFEnemyController::StopAI()
{
    UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
    if (BTComponent)
    {
        BTComponent->StopTree();
    }
}

void AIFEnemyController::SetTarget(APawn* Target)
{
    ensure(Target);
    BlackBoardPtr->SetValueAsObject(TEXT("Target"), Target);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *Target->GetName());
}

void AIFEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    RunAI();
}
