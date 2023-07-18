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

	class UIFHUD* GetHUD();

	void SetRewardHUD();

	UFUNCTION()
	void OpenPauseMenu();

protected:
	virtual void BeginPlay() override;

private:
	void ClosePauseMenu();

	UFUNCTION()
	void OpenGameOver();

	UFUNCTION()
	void OpenGameClear();
private:
	UPROPERTY()
	TObjectPtr<class UIFHUD> HUDWidget;

	TSubclassOf<class UIFHUD> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<class UIFPauseMenu> PauseWidget;
	
	TSubclassOf<class UIFPauseMenu> PauseWidgetClass;

	UPROPERTY()
	TObjectPtr<class UIFGameOver> GameOverWidget;

	TSubclassOf<class UIFGameOver> GameOverWidgetClass;

	UPROPERTY()
	TObjectPtr<class UCommonActivatableWidget> GameClearWidget;

	TSubclassOf<class UCommonActivatableWidget> GameClearWidgetClass;
};
