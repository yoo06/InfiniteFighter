// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "AI/IFEnemy.h"

UIFHpBarWidget::UIFHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    MaxHp = 100;
    CurrentHp = 100;
}

void UIFHpBarWidget::InitializeHp(float InMaxHp)
{
    MaxHp = InMaxHp;
    CurrentHp = MaxHp;
    HpBar->SetPercent(CurrentHp / MaxHp);
}

void UIFHpBarWidget::UpdateHp(float InHealth)
{
    CurrentHp = InHealth;
    HpBar->SetPercent(CurrentHp / MaxHp);
}
