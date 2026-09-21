// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "UnrealWorldHUD.h"
#include "ArchitectOption.generated.h"

/**
 * 
 */
UCLASS()
class UNREALWORLD_API UArchitectOption : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UImage* Image;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UButton* Button;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn;
	
	UFUNCTION(BlueprintCallable)
	void SpawnArchitectActor();
};
