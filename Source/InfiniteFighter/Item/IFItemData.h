// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IFItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Attack = 0,
	Heal,
};

/**
 * 
 */
UCLASS()
class INFINITEFIGHTER_API UIFItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("IFItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Description)
	FString ItemDescription;
};
