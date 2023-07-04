// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IFStageTable.h"
#include "IFStage.generated.h"

UCLASS()
class INFINITEFIGHTER_API AIFStage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIFStage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FORCEINLINE FIFStageTable GetStageTable(int32 InLevel) const { return StageTable.IsValidIndex(InLevel - 1) ? StageTable[InLevel - 1] : FIFStageTable(); }

	FORCEINLINE int32 GetCurrentLevel() const { return CurrentLevel; }

	UPROPERTY()
	int32 MaxLevel;


private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<class USceneComponent> SceneComponent;

	TArray<FIFStageTable> StageTable;

	int32 CurrentLevel;

	void SetStage();
};
