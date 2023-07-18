// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IFPlayerController.h"
#include "UI/IFHUD.h"
#include "UI/IFPauseMenu.h"
#include "UI/IFAimWidget.h"
#include "UI/IFGameOver.h"
#include "Interface/IFApplyItemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/IFItemButton.h"
#include "Item/IFItemData.h"
#include "IFCharacter.h"
#include "Stage/IFStage.h"
#include "Game/IFGameMode.h"
#include "CommonActivatableWidget.h"

AIFPlayerController::AIFPlayerController()
{
    static ConstructorHelpers::FClassFinder<UIFHUD>HUDWidgetClassRef
    (TEXT("/Game/InFiniteFighter/Widget/HUD/IFHUD.IFHUD_C"));
    if (HUDWidgetClassRef.Succeeded())
        HUDWidgetClass = HUDWidgetClassRef.Class;

    static ConstructorHelpers::FClassFinder<UIFPauseMenu>PauseWidgetClassRef
    (TEXT("/Game/InFiniteFighter/Widget/HUD/IFPause.IFPause_C"));
    if (PauseWidgetClassRef.Succeeded())
        PauseWidgetClass = PauseWidgetClassRef.Class;

    static ConstructorHelpers::FClassFinder<UIFGameOver>GameOverWidgetClassRef
    (TEXT("/Game/InFiniteFighter/Widget/HUD/IFGameover.IFGameover_C"));
    if (GameOverWidgetClassRef.Succeeded())
        GameOverWidgetClass = GameOverWidgetClassRef.Class;

    static ConstructorHelpers::FClassFinder<UCommonActivatableWidget>GameClearWidgetClassRef
    (TEXT("/Game/InFiniteFighter/Widget/HUD/IFClear.IFClear_C"));
    if (GameClearWidgetClassRef.Succeeded())
        GameClearWidgetClass = GameClearWidgetClassRef.Class;
}

UIFHUD* AIFPlayerController::GetHUD()
{
    return HUDWidget;
}

void AIFPlayerController::SetRewardHUD()
{
    HUDWidget->SetItemSlotVisible(true);
    FInputModeUIOnly UIOnlyInputMode;
    SetInputMode(UIOnlyInputMode);
}

void AIFPlayerController::OpenPauseMenu()
{
    UGameplayStatics::SetGamePaused(GetWorld(), true);

    PauseWidget->SetVisibility(ESlateVisibility::Visible);
    PauseWidget->ActivateWidget();
    FInputModeUIOnly UIOnlyInputMode;
    SetInputMode(UIOnlyInputMode);
}

void AIFPlayerController::BeginPlay()
{
    auto GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());
    if (::IsValid(GameMode))
    {
        GameMode->OnGameOver. BindUObject(this, &AIFPlayerController::OpenGameOver);
        GameMode->OnGameClear.BindUObject(this, &AIFPlayerController::OpenGameClear);
    }

    FInputModeGameOnly GameOnlyInputMode;
    SetInputMode(GameOnlyInputMode);

    // clamping camera
    PlayerCameraManager->ViewPitchMin = -60.0f;
    PlayerCameraManager->ViewPitchMax =  60.0f;

    // set GameClear widget
    GameClearWidget = CreateWidget<UCommonActivatableWidget>(this, GameClearWidgetClass);
    GameClearWidget->AddToViewport();
    GameClearWidget->SetVisibility(ESlateVisibility::Hidden);


    // set GameOver widget
    GameOverWidget = CreateWidget<UIFGameOver>(this, GameOverWidgetClass);
    GameOverWidget->AddToViewport();
    GameOverWidget->SetVisibility(ESlateVisibility::Hidden);

    GameOverWidget->Retry->OnClicked().AddUObject(this, &AIFPlayerController::RestartLevel);

    // set pause widget
    PauseWidget = CreateWidget<UIFPauseMenu>(this, PauseWidgetClass);
    PauseWidget->AddToViewport();
    PauseWidget->SetVisibility(ESlateVisibility::Hidden);

    PauseWidget->Cancel->OnClicked().AddUObject(this, &AIFPlayerController::ClosePauseMenu);


    // set HUD
    HUDWidget = CreateWidget<UIFHUD>(this, HUDWidgetClass);
    HUDWidget->AddToViewport();
    HUDWidget->GetAimWidget()->SetVisibility(ESlateVisibility::Hidden);
    HUDWidget->SetAxeIcon(false);
    HUDWidget->SetRecallIcon(false);
    HUDWidget->SetItemSlotVisible(false);

    for (const auto& Button : HUDWidget->Buttons)
    {
        Button->OnClicked().AddLambda([&]
            {
                IIFApplyItemInterface* PlayerCharacter = Cast<IIFApplyItemInterface>((GetCharacter()));
                if (PlayerCharacter)
                    PlayerCharacter->ApplyItem(Button->GetItem());

                HUDWidget->SetItemSlotVisible(false);

                AIFStage* Stage = Cast<AIFStage>(UGameplayStatics::GetActorOfClass(GetWorld(), AIFStage::StaticClass()));
                if (::IsValid(Stage))
                    Stage->SetStage();

                FInputModeGameOnly GameOnlyInputMode;
                SetInputMode(GameOnlyInputMode);
            });
    }
}

void AIFPlayerController::ClosePauseMenu()
{
    PauseWidget->SetVisibility(ESlateVisibility::Hidden);
    PauseWidget->DeactivateWidget();
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    FInputModeGameOnly GameOnlyInputMode;
    SetInputMode(GameOnlyInputMode);
}

void AIFPlayerController::OpenGameOver()
{
    GameOverWidget->SetVisibility(ESlateVisibility::Visible);
    GameOverWidget->ActivateWidget();
    FInputModeUIOnly UIOnlyInputMode;
    SetInputMode(UIOnlyInputMode);
}

void AIFPlayerController::OpenGameClear()
{
    GameClearWidget->SetVisibility(ESlateVisibility::Visible);
    GameClearWidget->ActivateWidget();
    FInputModeUIOnly UIOnlyInputMode;
    SetInputMode(UIOnlyInputMode);
}
