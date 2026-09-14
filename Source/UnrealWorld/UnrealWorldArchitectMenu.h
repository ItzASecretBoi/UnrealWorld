// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBlueprint.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UnrealWorldArchitectMenu.generated.h"

/**
 * 
 */
UCLASS()
class UNREALWORLD_API UUnrealWorldArchitectMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UButton> ArchitectMenuButton;
	
	
};
