// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_StunCheck.h"
#include "IFEnemy.h"
#include "IFEnemyAnimInstance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_StunCheck::UBTService_StunCheck()
{
    Interval = 0.1f;
}

void UBTService_StunCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	AIFEnemy* AIPawn = Cast<AIFEnemy>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsStunned"), AIPawn->AnimInstance->GetStunState());

	UE_LOG(LogTemp, Warning, TEXT("%d"), AIPawn->AnimInstance->GetStunState());
}
