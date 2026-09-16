// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchitectOption.h"


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
	UE_LOG(LogTemp, Log, TEXT("Hello from C++"));
}
