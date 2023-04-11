// Fill out your copyright notice in the Description page of Project Settings.


#include "IFAimWidget.h"
#include "Components/Image.h"

void UIFAimWidget::NativeConstruct()
{
    Aim = Cast<UImage>(GetWidgetFromName(TEXT("AimPoint")));
    AimTargetL = Cast<UImage>(GetWidgetFromName(TEXT("AimOnTarget_L")));
    AimTargetR = Cast<UImage>(GetWidgetFromName(TEXT("AimOnTarget_R")));
}

void UIFAimWidget::SetAimTargetUI(const bool& bIsTarget)
{
    if (bIsTarget)
    {
        AimTargetL->SetVisibility(ESlateVisibility::Visible);
        AimTargetR->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        AimTargetL->SetVisibility(ESlateVisibility::Hidden);
        AimTargetR->SetVisibility(ESlateVisibility::Hidden);
    }
}
