// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealWorldGameMode.h"
#include "UnrealWorldPlayerController.h"
#include "UnrealWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealWorldGameMode::AUnrealWorldGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AUnrealWorldPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_Player"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}