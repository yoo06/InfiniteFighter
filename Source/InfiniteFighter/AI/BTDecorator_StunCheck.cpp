// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_StunCheck.h"
#include "AIController.h"
#include "IFEnemy.h"
#include "IFEnemyAnimInstance.h"

UBTDecorator_StunCheck::UBTDecorator_StunCheck()
{
    NodeName = TEXT("Stun Check");
}

bool UBTDecorator_StunCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    AIFEnemy* ControllingPawn = Cast<AIFEnemy>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == ControllingPawn)
    {
        return false;
    }

    bResult = ControllingPawn->AnimInstance->GetStunState();

    return bResult;
}
