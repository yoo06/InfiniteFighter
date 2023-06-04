// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AttackCheck.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Attack Check"))
class INFINITEFIGHTER_API UAnimNotifyState_AttackCheck : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
		float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StartSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EndSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SphereSize;
};