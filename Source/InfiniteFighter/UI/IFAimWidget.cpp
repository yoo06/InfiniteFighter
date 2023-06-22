// Fill out your copyright notice in the Description page of Project Settings.


#include "IFAimWidget.h"
#include "Components/Image.h"

void UIFAimWidget::NativeConstruct()
{

}

void UIFAimWidget::SetAimTargetUI(const bool& bIsTarget)
{
    if (bIsTarget)
    {
        AimOnTargetL->SetVisibility(ESlateVisibility::Visible);
        AimOnTargetR->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        AimOnTargetL->SetVisibility(ESlateVisibility::Hidden);
        AimOnTargetR->SetVisibility(ESlateVisibility::Hidden);
    }
}

const bool UIFAimWidget::GetAimTargetUI() const
{
    return (AimOnTargetL->GetVisibility() == ESlateVisibility::Hidden);
}
