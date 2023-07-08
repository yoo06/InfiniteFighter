// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFIconWidget.h"
#include "Components/Image.h"
#include "CommonActionWidget.h"

void UIFIconWidget::NativeConstruct()
{

}

void UIFIconWidget::SetAxeIcon(bool bIsAxeHolding)
{
    if (bIsAxeHolding)
    {
        Axe_Icon->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        Axe_Icon->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UIFIconWidget::SetRecallIcon(bool bIsAxeThrown)
{
    if (bIsAxeThrown)
    {
        RecallWidget->SetHidden(false);
    }
    else
    {
        RecallWidget->SetHidden(true);
    }
}
