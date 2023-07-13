// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/IFItemData.h"
#include "IFAttackItemData.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UIFAttackItemData : public UIFItemData
{
	GENERATED_BODY()
	
public:
	UIFAttackItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("IFItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Stat)
	float IncreaseAttackRate;
};
