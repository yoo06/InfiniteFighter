// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "IFPauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UIFPauseMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonButtonBase> Cancel;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonButtonBase> BackToMainMenu;
};
