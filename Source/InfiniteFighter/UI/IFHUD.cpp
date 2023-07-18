// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFHUD.h"
#include "IFHpBarWidget.h"
#include "IFIconWidget.h"
#include "IFAimWidget.h"
#include "IFItemButton.h"
#include "Components/Image.h"

UIFHUD::UIFHUD(const FObjectInitializer& ObjectInitializer)
{
}

void UIFHUD::NativeConstruct()
{
    Buttons.Add(IFItemButton);
    Buttons.Add(IFItemButton1);
    Buttons.Add(IFItemButton2);
}

void UIFHUD::SetAxeIcon(bool bIsAxeHolding)
{
    IconWidget->SetAxeIcon(bIsAxeHolding);
}

void UIFHUD::SetRecallIcon(bool bIsAxeThrown)
{
    IconWidget->SetRecallIcon(bIsAxeThrown);
}

void UIFHUD::InitializeHp(float InMaxHp)
{
    HpBar->InitializeHp(InMaxHp);
}

void UIFHUD::UpdateHp(float InHealth)
{
    HpBar->UpdateHp(InHealth);
}

void UIFHUD::SetItemSlotVisible(bool bIsVisible)
{
    if (bIsVisible)
    {
        for (const auto& Button : Buttons)
        {
            Button->SetItem();
            Button->SetVisibility(ESlateVisibility::Visible);
            BackGround->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        for (const auto& Button : Buttons)
        {
            Button->SetVisibility(ESlateVisibility::Hidden);
            BackGround->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    Buttons[0]->SetFocus();
}

UIFAimWidget* UIFHUD::GetAimWidget()
{
    return AimWidget;
}
