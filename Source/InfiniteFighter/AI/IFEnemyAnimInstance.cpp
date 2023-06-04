// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IFEnemyAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "IFEnemy.h"

UIFEnemyAnimInstance::UIFEnemyAnimInstance()
{
    EnemyDirection = 0.0f;
    EnemySpeed     = 0.0f;

    static ConstructorHelpers::FObjectFinder<UAnimMontage>REACT_BACK_MONTAGE
    (TEXT("/Game/InFiniteFighter/AI/Animation/React/hit_heavy_back_Montage.hit_heavy_back_Montage"));
    if (REACT_BACK_MONTAGE.Succeeded())
        ReactBackMontage = REACT_BACK_MONTAGE.Object;

    static ConstructorHelpers::FObjectFinder<UAnimMontage>REACT_FRONT_MONTAGE
    (TEXT("/Game/InFiniteFighter/AI/Animation/React/hit_heavy_front_Montage.hit_heavy_front_Montage"));
    if (REACT_FRONT_MONTAGE.Succeeded())
        ReactFrontMontage = REACT_FRONT_MONTAGE.Object;

    static ConstructorHelpers::FObjectFinder<UAnimMontage>ATTACK_MONTAGE
    (TEXT("/Game/InFiniteFighter/AI/Animation/Combat/atk_04_Montage.atk_04_Montage"));
    if (ATTACK_MONTAGE.Succeeded())
        AttackMontage = ATTACK_MONTAGE.Object;

    bIsStunned = false;
}

void UIFEnemyAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
}

void UIFEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    auto Pawn = TryGetPawnOwner();

    if (::IsValid(Pawn))
    {
        // get speed
        EnemySpeed = Pawn->GetVelocity().Size();

        // get direction
        EnemyDirection = UKismetAnimationLibrary::CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
    }
}
void UIFEnemyAnimInstance::React(AActor* Target, AActor* Causer)
{
    if (!(::IsValid(Target) && ::IsValid(Causer))) return;
    // check if character and enemy are facing(DotProduct on both character's forward vector)
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

void UIFEnemyAnimInstance::PlayAttackMontage()
{
    Montage_Play(AttackMontage);
}
