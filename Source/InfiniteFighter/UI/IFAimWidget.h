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

	FORCEINLINE void SetAimTargetUI(const bool& bIsTarget);
	FORCEINLINE const bool GetAimTargetUI() const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> AimPoint;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> AimOnTargetL;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> AimOnTargetR;
};
