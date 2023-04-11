// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IFAimWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UIFAimWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetAimTargetUI(const bool& bIsTarget);

private:
	UPROPERTY()
	TObjectPtr<class UImage> Aim;

	UPROPERTY()
	TObjectPtr<class UImage> AimTargetL;

	UPROPERTY()
	TObjectPtr<class UImage> AimTargetR;
};
