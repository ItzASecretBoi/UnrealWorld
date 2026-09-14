// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealWorldArchitectMenu.h"
#include "GameFramework/HUD.h"
#include "Components/Button.h"
#include "UnrealWorldHUD.generated.h"

/**
 * 
 */
 
 class UButton;
 
UCLASS()
class UNREALWORLD_API AUnrealWorldHUD : public AHUD
{
	GENERATED_BODY()
	
	
public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UUnrealWorldArchitectMenu> ArchitectMenu;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUnrealWorldArchitectMenu> ArchitectMenuClass;
	
protected:
	virtual void BeginPlay() override;
};