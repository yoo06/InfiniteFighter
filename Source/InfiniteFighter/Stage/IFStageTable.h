
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "IFStageTable.generated.h"

USTRUCT(BlueprintType)
struct FIFStageTable : public FTableRowBase
{
    GENERATED_BODY()
    
public:
    FIFStageTable() : MaxHp(0.0f), Attack(0.0f), Count(0) {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stage)
    float MaxHp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stage)
    float Attack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stage)
    int32 Count;
};