// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFItemButton.h"
#include "Components/TextBlock.h"
#include "Item/IFItemData.h"
#include "Engine/AssetManager.h"

void UIFItemButton::NativeConstruct()
{
    ItemName->SetText(FText::FromString(TEXT("ItemNameTemp")));
    ItemDescription->SetText(FText::FromString(TEXT("ItemDescriptionTemp")));
}

void UIFItemButton::SetItem()
{
	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("IFItemData"), Assets);
	UE_LOG(LogTemp, Warning, TEXT("%d"), Assets.Num());
	ensure(0 < Assets.Num());

	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}
	Item = Cast<UIFItemData>(AssetPtr.Get());

    ItemName->SetText(FText::FromString(Item->ItemName));
    ItemDescription->SetText(FText::FromString(Item->ItemDescription));
}
