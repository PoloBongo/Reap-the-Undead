// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReapTheUndeadGameMode.h"
#include "ReapTheUndeadCharacter.h"
#include "UObject/ConstructorHelpers.h"

AReapTheUndeadGameMode::AReapTheUndeadGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
