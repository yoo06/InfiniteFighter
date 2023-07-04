// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/IFStage.h"
#include "AI/IFEnemy.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AIFStage::AIFStage()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	static ConstructorHelpers::FObjectFinder<UDataTable> StageDataTable
	(TEXT("/Game/InFiniteFighter/Miscellaneous/DataTable/DT_StageTable.DT_StageTable"));
	if (StageDataTable.Succeeded())
	{
		const UDataTable* DataTable = StageDataTable.Object;

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, StageTable, [](uint8* Value) { return *reinterpret_cast<FIFStageTable*>(Value); });
	}

	MaxLevel = StageTable.Num();

	CurrentLevel = 1;
}

void AIFStage::BeginPlay()
{
	SetStage();
}

void AIFStage::SetStage()
{ 
	int32 SpawnNumber = StageTable[CurrentLevel].Count;
	for (int i = 0; i < SpawnNumber; ++i)
	{
		float Radius = 750.0f;
		FVector RandomPoint = GetActorLocation() + UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.0f, Radius);
		// RandomPoint.Z = 0;
		AIFEnemy* Enemy = GetWorld()->SpawnActorAbsolute<AIFEnemy>(RandomPoint, FRotator::ZeroRotator);
		Enemy->SetEnemy(StageTable[CurrentLevel].MaxHp, StageTable[CurrentLevel].Attack);
	}
}
