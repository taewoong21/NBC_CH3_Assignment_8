// Copyright Epic Games, Inc. All Rights Reserved.

#include "NBCGameMode.h"
#include "NBCCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANBCGameMode::ANBCGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
