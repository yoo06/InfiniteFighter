// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_StunCheck.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UBTDecorator_StunCheck : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_StunCheck();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
