// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API AIFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AIFPlayerController();

	class UIFHUDWidget* GetHUD();

	void SetRewardHUD();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<class UIFHUDWidget> HUDWidget;

	TSubclassOf<class UIFHUDWidget> HUDWidgetClass;
};
