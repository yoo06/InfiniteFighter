// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFGameOver.h"
#include "CommonButtonBase.h"
#include "Kismet/GameplayStatics.h"

void UIFGameOver::NativeConstruct()
{
    OnActivated().AddLambda([&] { BackToMainMenu->SetFocus(); });
}
