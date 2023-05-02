// Fill out your copyright notice in the Description page of Project Settings.


#include "IFGameMode.h"
#include "Character/IFCharacter.h"

AIFGameMode::AIFGameMode()
{
	DefaultPawnClass = AIFCharacter::StaticClass();
}
