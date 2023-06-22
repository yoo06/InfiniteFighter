// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_RandomChance.h"

UBTDecorator_RandomChance::UBTDecorator_RandomChance()
{
    NodeName = TEXT("Random");
}

bool UBTDecorator_RandomChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    float Rand = FMath::FRand();

    return SucceedRate >= Rand;
}
