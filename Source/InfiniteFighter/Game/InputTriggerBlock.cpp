// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/InputTriggerBlock.h"
#include "InputAction.h"
#include "EnhancedPlayerInput.h"


ETriggerState UInputTriggerBlock::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	const FInputActionValue ActionValue = PlayerInput->GetActionValue(BlockAction);

	return IsActuated(ActionValue) ? ETriggerState::Triggered : ETriggerState::None;
}