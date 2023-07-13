// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IFPlayerController.h"
#include "UI/IFHUDWidget.h"
#include "UI/IFAimWidget.h"
#include "Interface/IFApplyItemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/IFItemButton.h"
#include "Item/IFItemData.h"
#include "IFCharacter.h"
#include "Stage/IFStage.h"

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

void AIFPlayerController::SetRewardHUD()
{
    HUDWidget->SetItemSlotVisible(true);
    FInputModeUIOnly UIOnlyInputMode;
    SetInputMode(UIOnlyInputMode);
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
    HUDWidget->SetItemSlotVisible(false);

    for (const auto& Button : HUDWidget->Buttons)
    {
        Button->OnClicked().AddLambda([&]
            {
                IIFApplyItemInterface* PlayerCharacter = Cast<IIFApplyItemInterface>((GetCharacter()));
                if(PlayerCharacter)
                    PlayerCharacter->ApplyItem(Button->GetItem());

                HUDWidget->SetItemSlotVisible(false);

                AIFStage* Stage = Cast<AIFStage>(UGameplayStatics::GetActorOfClass(GetCharacter()->GetWorld(), AIFStage::StaticClass()));
                if (::IsValid(Stage))
                    Stage->SetStage();

                FInputModeGameOnly GameOnlyInputMode;
                SetInputMode(GameOnlyInputMode);
            });
    }
}
