// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "UnrealWorldCharacter.h"
#include "UnrealWorldPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AUnrealWorldPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUnrealWorldPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Camera Pan Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* PanAction;
	
	/** Zoom In*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ZoomInAction;
	
	/** Zoom Out*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ZoomOutAction;
	
	/** PrimaryClick Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* PrimaryCliok;

	/** SecondaryClick */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SecondaryClick;
	
	UPROPERTY(BlueprintReadOnly)
	APawn* ControlledPawn;
	
	UPROPERTY(BlueprintReadOnly)
	AActor* SelectedActor;
	
protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnPrimaryInputStarted();
	void OnPrimaryTriggered();
	void OnPrimaryReleased();
	
	void OnSecondaryInputStarted();
	void OnSecondaryTriggered();
	void OnSecondaryReleased();
	
	void OnPanTriggered();
	void OnPanReleased();

	void ZoomIn();
	void ZoomOut();
	
private:
	FVector CachedDestination;

	float FollowTime; // For how long it has been pressed
	
	
	bool bIsPanning = false;
	FVector2D LastPanMousePosition = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, Category = Camera)
	float PanSpeed = 1.0f;
};


