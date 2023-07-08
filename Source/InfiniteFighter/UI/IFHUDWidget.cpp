// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFHUDWidget.h"
#include "IFHpBarWidget.h"
#include "IFIconWidget.h"
#include "IFAimWidget.h"

void UIFHUDWidget::SetAxeIcon(bool bIsAxeHolding)
{
    IconWidget->SetAxeIcon(bIsAxeHolding);
}

void UIFHUDWidget::SetRecallIcon(bool bIsAxeThrown)
{
    IconWidget->SetRecallIcon(bIsAxeThrown);
}

void UIFHUDWidget::InitializeHp(float InMaxHp)
{
    HpBar->InitializeHp(InMaxHp);
}

void UIFHUDWidget::UpdateHp(float InHealth)
{
    HpBar->UpdateHp(InHealth);
}

UIFAimWidget* UIFHUDWidget::GetAimWidget()
{
    return AimWidget;
}
