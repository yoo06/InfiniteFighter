// Fill out your copyright notice in the Description page of Project Settings.


#include "IFGameMode.h"
#include "Character/IFCharacter.h"

AIFGameMode::AIFGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef
	(TEXT("/Script/InfiniteFighter.IFCharacter"));
	if(ThirdPersonClassRef.Succeeded())
		DefaultPawnClass = ThirdPersonClassRef.Class;
}
