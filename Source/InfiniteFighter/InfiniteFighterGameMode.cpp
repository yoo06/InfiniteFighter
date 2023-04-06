// Copyright Epic Games, Inc. All Rights Reserved.

#include "InfiniteFighterGameMode.h"
#include "InfiniteFighterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInfiniteFighterGameMode::AInfiniteFighterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
