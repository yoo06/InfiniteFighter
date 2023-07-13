// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IFHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UIFHpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UIFHpBarWidget(const FObjectInitializer& ObjectInitializer);

	void InitializeHp(float InMaxHp);

	void UpdateHp(float InHealth);
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
