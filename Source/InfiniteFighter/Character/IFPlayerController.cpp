// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IFPlayerController.h"
#include "UI/IFHUDWidget.h"
#include "UI/IFAimWidget.h"

AIFPlayerController::AIFPlayerController()
{
    static ConstructorHelpers::FClassFinder<UIFHUDWidget>HUDWidgetClassRef
    (TEXT("/Game/InFiniteFighter/Widget/HUD/IFHUDWidget.IFHUDWidget_C"));
    if (HUDWidgetClassRef.Succeeded())
        HUDWidgetClass = HUDWidgetClassRef.Class;
}

UIFHUDWidget* AIFPlayerController::GetHUD()
{
    return HUDWidget;
}

void AIFPlayerController::BeginPlay()
{
    FInputModeGameOnly GameOnlyInputMode;
    SetInputMode(GameOnlyInputMode);

    PlayerCameraManager->ViewPitchMin = -60.0f;
    PlayerCameraManager->ViewPitchMax =  60.0f;

    HUDWidget = CreateWidget<UIFHUDWidget>(this, HUDWidgetClass);

    HUDWidget->AddToViewport();

    HUDWidget->GetAimWidget()->SetVisibility(ESlateVisibility::Hidden);
    HUDWidget->SetAxeIcon(false);
    HUDWidget->SetRecallIcon(false);
}
