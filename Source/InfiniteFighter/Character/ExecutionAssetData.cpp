// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ExecutionAssetData.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"

UExecutionAssetData::UExecutionAssetData()
{
	WarpPoint = FVector::ZeroVector;
}

void UExecutionAssetData::CreateSequencePlayer()
{
	if (LevelSequence)
	{
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.bDisableLookAtInput = true;
		Settings.bDisableMovementInput = true;
		Settings.bHideHud = true;

		ALevelSequenceActor* SequenceActor;

		LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GWorld, LevelSequence, Settings, SequenceActor);
	}
}

void UExecutionAssetData::Play()
{
	LevelSequencePlayer->Play();
}
