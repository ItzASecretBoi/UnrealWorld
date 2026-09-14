// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealWorldHUD.h"

#include "UnrealWorldArchitectMenu.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"

void AUnrealWorldHUD::BeginPlay() 
{
	ArchitectMenu = CreateWidget<UUnrealWorldArchitectMenu>(GetOwningPlayerController(), ArchitectMenuClass);
	
	
	if (ArchitectMenu)
	{
		ArchitectMenu->AddToViewport();
	}
	
	if (!ArchitectMenu)
	{
		UE_LOG(LogTemp, Error, TEXT("Architect menu: CreateWidget failed."));
		return;
	}
	
}

