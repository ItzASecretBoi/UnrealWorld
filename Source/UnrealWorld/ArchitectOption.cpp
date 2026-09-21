// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealWorldHUD.h"
#include "UnrealWorldCharacter.h"
#include "UnrealWorldPlayer.h"
#include "ArchitectOption.h"

#include "UnrealWorldPlayerController.h"


void UArchitectOption::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button))
	{
		Button->OnClicked.AddDynamic(this, &UArchitectOption::SpawnArchitectActor);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ArchitectOption: Button was not bound. "
			"Ensure the Widget Blueprint button is named 'Button' and marked as a variable."));
	}
}

void UArchitectOption::SpawnArchitectActor()
{

	AUnrealWorldPlayer* Player = Cast<AUnrealWorldPlayer>(GetOwningPlayer()->GetPawn());
	AUnrealWorldPlayerController* PlayerController = Cast<AUnrealWorldPlayerController>(Player->GetController());
	if (Player)
	{
		PlayerController->SelectedInteractableClass = ActorToSpawn;
	}
}
