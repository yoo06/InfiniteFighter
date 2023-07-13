// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFHUDWidget.h"
#include "IFHpBarWidget.h"
#include "IFIconWidget.h"
#include "IFAimWidget.h"
#include "IFItemButton.h"

UIFHUDWidget::UIFHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UIFHUDWidget::NativeConstruct()
{
    Buttons.Add(IFItemButton);
    Buttons.Add(IFItemButton1);
    Buttons.Add(IFItemButton2);
}

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

void UIFHUDWidget::SetItemSlotVisible(bool bIsVisible)
{
    for (const auto& Button : Buttons)
    {
        if (bIsVisible)
        {
            Button->SetItem();
            Button->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            Button->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

UIFAimWidget* UIFHUDWidget::GetAimWidget()
{
    return AimWidget;
}
