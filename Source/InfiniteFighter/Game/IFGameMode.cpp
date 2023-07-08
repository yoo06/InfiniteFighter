// Fill out your copyright notice in the Description page of Project Settings.


#include "IFGameMode.h"

AIFGameMode::AIFGameMode()
{
    static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef
    (TEXT("/Script/InfiniteFighter.IFCharacter"));
    if (ThirdPersonClassRef.Succeeded())
        DefaultPawnClass = ThirdPersonClassRef.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef
	(TEXT("/Script/InfiniteFighter.IFPlayerController"));
    if (PlayerControllerRef.Succeeded())
        PlayerControllerClass = PlayerControllerRef.Class;
}
