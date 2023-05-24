// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IFEnemyAnimInstance.h"

UIFEnemyAnimInstance::UIFEnemyAnimInstance()
{
    static ConstructorHelpers::FObjectFinder<UAnimMontage>REACT_BACK_MONTAGE
    (TEXT("/Game/InFiniteFighter/AI/Animation/React/hit_heavy_back_Montage.hit_heavy_back_Montage"));
    if (REACT_BACK_MONTAGE.Succeeded())
        ReactBackMontage = REACT_BACK_MONTAGE.Object;

    static ConstructorHelpers::FObjectFinder<UAnimMontage>REACT_FRONT_MONTAGE
    (TEXT("/Game/InFiniteFighter/AI/Animation/React/hit_heavy_front_Montage.hit_heavy_front_Montage"));
    if (REACT_FRONT_MONTAGE.Succeeded())
        ReactFrontMontage = REACT_FRONT_MONTAGE.Object;
}

void UIFEnemyAnimInstance::React(AActor* Target, AActor* Causer)
{
    float DotProduct = FVector::DotProduct(Causer->GetActorForwardVector(), Target->GetActorForwardVector());
    
    FRotator CurrentRotation = Target->GetActorRotation();
    if (DotProduct < 0)
    {
        Target->SetActorRotation(FRotator(CurrentRotation.Pitch, Causer->GetActorRotation().Yaw + 180, CurrentRotation.Roll));
        Montage_Play(ReactFrontMontage);
    }
    else
    {
        Target->SetActorRotation(FRotator(CurrentRotation.Pitch, Causer->GetActorRotation().Yaw, CurrentRotation.Roll));
        Montage_Play(ReactBackMontage);
    }
}