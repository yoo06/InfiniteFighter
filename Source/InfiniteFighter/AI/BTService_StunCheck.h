// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_StunCheck.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UBTService_StunCheck : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_StunCheck();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


};
