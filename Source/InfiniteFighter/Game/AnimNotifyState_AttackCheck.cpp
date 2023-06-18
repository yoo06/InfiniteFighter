// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AnimNotifyState_AttackCheck.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotifyState_AttackCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
    const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

    APawn* MeshOwner = Cast<APawn>(MeshComp->GetOwner());

    if (::IsValid(MeshOwner))
    {
        FHitResult OutHit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(MeshOwner);
        bool bResult = MeshOwner->GetWorld()->SweepSingleByChannel
        (
            OutHit,
            MeshComp->GetSocketLocation(StartSocket),
            MeshComp->GetSocketLocation(EndSocket),
            FQuat::Identity,
            ECollisionChannel::ECC_Visibility,
            FCollisionShape::MakeSphere(SphereSize),
            Params
        );
        if (bResult)
        {
            APawn* HitTarget = Cast<APawn>(OutHit.GetActor());
            if (::IsValid(HitTarget))
            {
                FDamageEvent DamageEvent;
                if (HitTarget->TakeDamage(1, DamageEvent, MeshOwner->GetController(), MeshOwner) > 0)
                    UGameplayStatics::SpawnEmitterAtLocation(MeshOwner->GetWorld(), BloodParticle, OutHit.ImpactPoint, FRotator::ZeroRotator, true);
            }
        }

 //#if ENABLE_DRAW_DEBUG
 //        DrawDebugSphere(MeshOwner->GetWorld(), (MeshComp->GetSocketLocation(StartSocket) + MeshComp->GetSocketLocation(EndSocket)) / 2, SphereSize,
 //            12, bResult? FColor::Green : FColor::Red, false, 5.0f);
 //#endif
 #if ENABLE_DRAW_DEBUG
         DrawDebugSphere(MeshOwner->GetWorld(), (MeshComp->GetSocketLocation(StartSocket) + MeshComp->GetSocketLocation(EndSocket)) / 2, SphereSize,
             12, bResult? FColor::Green : FColor::Red, false, 5.0f);
 #endif
    }
}