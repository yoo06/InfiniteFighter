// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IFHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UIFHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetAxeIcon(bool bIsAxeHolding);

	void SetRecallIcon(bool bIsAxeThrown);

	void InitializeHp(float InMaxHp);

	void UpdateHp(float InHealth);

	UFUNCTION()
	UIFAimWidget* GetAimWidget();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UIFIconWidget> IconWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UIFHpBarWidget> HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UIFAimWidget> AimWidget;
};
