// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IFGameMode.generated.h"

DECLARE_DELEGATE(FOnGameOverDelegate)
DECLARE_DELEGATE(FOnGameClearDelegate)
/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API AIFGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AIFGameMode();

	FOnGameOverDelegate OnGameOver;

	FOnGameClearDelegate OnGameClear;
};
