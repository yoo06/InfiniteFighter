// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFPauseMenu.h"
#include "CommonButtonBase.h"
#include "Kismet/GameplayStatics.h"

void UIFPauseMenu::NativeConstruct()
{
    OnActivated().AddLambda([&] { BackToMainMenu->SetFocus(); });
}
