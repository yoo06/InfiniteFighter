// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "IFGameOver.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UIFGameOver : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonButtonBase> Retry;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonButtonBase> BackToMainMenu;

};
