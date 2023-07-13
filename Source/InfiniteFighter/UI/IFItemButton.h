// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "IFItemButton.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UIFItemButton : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetItem();

	FORCEINLINE class UIFItemData* GetItem() { return Item; }

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemDescription;

	TObjectPtr<class UIFItemData> Item;
};
