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
	UIFHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetAxeIcon(bool bIsAxeHolding);

	void SetRecallIcon(bool bIsAxeThrown);

	void InitializeHp(float InMaxHp);

	void UpdateHp(float InHealth);

	UFUNCTION()
	void SetItemSlotVisible(bool bIsVisible);

	UFUNCTION()
	UIFAimWidget* GetAimWidget();

	UPROPERTY()
	TArray<class UIFItemButton*> Buttons;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UIFIconWidget> IconWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UIFHpBarWidget> HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UIFAimWidget> AimWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UIFItemButton> IFItemButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UIFItemButton> IFItemButton1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UIFItemButton> IFItemButton2;
};
