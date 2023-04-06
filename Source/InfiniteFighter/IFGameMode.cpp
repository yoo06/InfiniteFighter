// Fill out your copyright notice in the Description page of Project Settings.


#include "IFGameMode.h"
#include "IFCharacter.h"

AIFGameMode::AIFGameMode()
{
	DefaultPawnClass = AIFCharacter::StaticClass();
}
