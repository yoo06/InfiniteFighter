// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_StunCheck.h"
#include "GameplayTags/EnemyTag.h"
#include "AIController.h"
#include "GameplayTagAssetInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_StunCheck::UBTService_StunCheck()
{
    Interval = 0.2f;
}

void UBTService_StunCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	IGameplayTagAssetInterface* AIPawn = Cast<IGameplayTagAssetInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsStunned"), AIPawn->HasMatchingGameplayTag(ENEMY_STUN));
}
