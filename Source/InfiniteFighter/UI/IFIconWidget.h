// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IFIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UIFIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetAxeIcon(bool bIsAxeHolding);

	void SetRecallIcon(bool bIsAxeThrown);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Axe_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonActionWidget> RecallWidget;
};
