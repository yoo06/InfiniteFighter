// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/IFStage.h"
#include "AI/IFEnemy.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Character/IFPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "CommonUserWidget.h"
#include "Game/IFGameMode.h"

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

	MaxLevel = StageTable.Num()- 1;
	// MaxLevel = 2;

	// setting Stage Blocker
	StageBlockerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StageBlocker"));
	StageBlockerBox->SetupAttachment(SceneComponent);
	StageBlockerBox->SetRelativeLocation(FVector(0.0f, 1000.0f, 0.0f));
	StageBlockerBox->SetBoxExtent(FVector(400.0f, 200.0f, 800.0f));
	StageBlockerBox->SetCollisionProfileName(TEXT("NoCollision"));

	// setting Stage Trigger Box
	StageTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTriggerBox->SetupAttachment(SceneComponent);
	StageTriggerBox->SetRelativeLocation(FVector(-1200.0f, 250.0f, -350.0f));
	StageTriggerBox->SetBoxExtent(FVector(150.0f, 150.0f, 150.0f));
	
	// setting Interaction
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(StageTriggerBox);
	InteractionWidget->SetRelativeLocation(FVector(0.0f, 300.0f, 30.0f));

	static ConstructorHelpers::FClassFinder<UCommonUserWidget>InteractionWidgetRef
	(TEXT("/Game/InFiniteFighter/Widget/Game/Interaction.Interaction_C"));
	if (::IsValid(InteractionWidgetRef.Class))
	{
		InteractionWidget->SetWidgetClass(InteractionWidgetRef.Class);
		InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
		InteractionWidget->SetDrawSize(FVector2D(50.0f, 50.0f));
		InteractionWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	bCanGameStart = false;
	CurrentLevel = 0;
}

void AIFStage::BeginPlay()
{
	Super::BeginPlay();

	InteractionWidget->SetVisibility(false);
}

void AIFStage::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StageTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AIFStage::OnTriggerBoxBeginOverlap);
	StageTriggerBox->OnComponentEndOverlap.  AddDynamic(this, &AIFStage::OnTriggerBoxEndOverlap);
}

void AIFStage::SetStage()
{ 
	SpawnNumber = StageTable[CurrentLevel].Count;
	UE_LOG(LogTemp, Warning, TEXT("%d"), SpawnNumber);

	for (int i = 0; i < SpawnNumber; ++i)
	{
		float Radius = 1000.0f;
		float VectorZ = GetActorLocation().Z;
		FVector RandomPoint = GetActorLocation() + UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(500.0f, Radius);
		RandomPoint.Z = VectorZ + 88.0f;
		FTransform SpawnTransform(RandomPoint);
		AIFEnemy* Enemy = GetWorld()->SpawnActor<AIFEnemy>(RandomPoint, FRotator::ZeroRotator);
		if(::IsValid(Enemy))
		{
			Enemy->OnDestroyed.AddDynamic(this, &AIFStage::SetReward);
			Enemy->SetEnemy(StageTable[CurrentLevel].MaxHp, StageTable[CurrentLevel].Attack);
			UE_LOG(LogTemp, Warning, TEXT("EnemyLeft : %d"), SpawnNumber);
		}
	}
}

void AIFStage::StartGame()
{
	if (bCanGameStart)
	{
		StageBlockerBox->SetCollisionProfileName(TEXT("InvisibleWall"));
		StageTriggerBox->SetCollisionProfileName(TEXT("NoCollision"));
		InteractionWidget->SetVisibility(false);
		bCanGameStart = false;
		SetStage();
	}
}

void AIFStage::OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		InteractionWidget->SetVisibility(true);
		bCanGameStart = true;
	}
}

void AIFStage::OnTriggerBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		InteractionWidget->SetVisibility(false);
		bCanGameStart = false;
	}
}

void AIFStage::SetReward(AActor* DestroyedActor)
{
	SpawnNumber--;
	UE_LOG(LogTemp, Warning, TEXT("EnemyLeft : %d"), SpawnNumber);

	if (SpawnNumber == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("StageEnd"));
		if (CurrentLevel == MaxLevel)
		{
			auto GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());
			if (::IsValid(GameMode))
			{
				GameMode->OnGameClear.ExecuteIfBound();
				return;
			}
		}
		CurrentLevel++;
		auto PlayerController = Cast<AIFPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (::IsValid(PlayerController))
		{
			PlayerController->SetRewardHUD();
		}
	}
}
