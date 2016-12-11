// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Doppel.h"
#include "DoppelGameMode.h"
#include "DoppelPlayerController.h"
#include "DoppelCharacter.h"

ADoppelGameMode::ADoppelGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADoppelPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}