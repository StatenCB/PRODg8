// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResidenceEvilGameMode.h"
#include "ResidenceEvilCharacter.h"
#include "UObject/ConstructorHelpers.h"

AResidenceEvilGameMode::AResidenceEvilGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
