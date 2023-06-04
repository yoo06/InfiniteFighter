// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "InputTriggerBlock.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Block", NotInputConfigurable = "true"))
class INFINITEFIGHTER_API UInputTriggerBlock : public UInputTrigger
{
	GENERATED_BODY()
	
public:
	virtual ETriggerType GetTriggerType_Implementation() const override { return ETriggerType::Blocker; }

	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;

	// Actions that will cancel the combo if it is triggered
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings", meta = (DisplayThumbnail = "false"))
	TObjectPtr<const UInputAction> BlockAction = nullptr;
};
