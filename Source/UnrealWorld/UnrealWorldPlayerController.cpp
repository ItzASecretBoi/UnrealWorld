// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealWorldPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "UnrealWorldCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


AUnrealWorldPlayerController::AUnrealWorldPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AUnrealWorldPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	ControlledPawn = GetPawn();
}

void AUnrealWorldPlayerController::OnPrimaryInputStarted()
{
}

void AUnrealWorldPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AUnrealWorldPlayerController::OnInputStarted);
		
		EnhancedInputComponent->BindAction(PrimaryCliok, ETriggerEvent::Started, this, &AUnrealWorldPlayerController::OnPrimaryInputStarted);
		EnhancedInputComponent->BindAction(PrimaryCliok, ETriggerEvent::Triggered, this, &AUnrealWorldPlayerController::OnPrimaryTriggered);
		EnhancedInputComponent->BindAction(PrimaryCliok, ETriggerEvent::Completed, this, &AUnrealWorldPlayerController::OnPrimaryReleased);
		EnhancedInputComponent->BindAction(PrimaryCliok, ETriggerEvent::Canceled, this, &AUnrealWorldPlayerController::OnPrimaryReleased);
		
		EnhancedInputComponent->BindAction(SecondaryClick, ETriggerEvent::Started, this, &AUnrealWorldPlayerController::OnSecondaryInputStarted);
		EnhancedInputComponent->BindAction(SecondaryClick, ETriggerEvent::Triggered, this, &AUnrealWorldPlayerController::OnSecondaryTriggered);
		EnhancedInputComponent->BindAction(SecondaryClick, ETriggerEvent::Completed, this, &AUnrealWorldPlayerController::OnSecondaryReleased);
		EnhancedInputComponent->BindAction(SecondaryClick, ETriggerEvent::Canceled, this, &AUnrealWorldPlayerController::OnSecondaryReleased);
		
		//PanAction
		//EnhancedInputComponent->BindAction(PanAction, ETriggerEvent::Started, this, &AUnrealWorldPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(PanAction, ETriggerEvent::Triggered, this, &AUnrealWorldPlayerController::OnPanTriggered);
		EnhancedInputComponent->BindAction(PanAction, ETriggerEvent::Completed, this, &AUnrealWorldPlayerController::OnPanReleased);
		EnhancedInputComponent->BindAction(PanAction, ETriggerEvent::Canceled, this, &AUnrealWorldPlayerController::OnPanReleased);
		
		EnhancedInputComponent->BindAction(ZoomInAction, ETriggerEvent::Triggered, this, &AUnrealWorldPlayerController::ZoomIn);
		EnhancedInputComponent->BindAction(ZoomOutAction, ETriggerEvent::Triggered, this, &AUnrealWorldPlayerController::ZoomOut);
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AUnrealWorldPlayerController::OnPrimaryTriggered()
{
	FHitResult Hit;
	const bool bHitSuccessful = GetHitResultUnderCursor(ECC_Visibility, true, Hit);

	if (!bHitSuccessful)
	{
		return;
	}

	AUnrealWorldCharacter* NewUnit = Cast<AUnrealWorldCharacter>(Hit.GetActor());
	AUnrealWorldCharacter* OldUnit = Cast<AUnrealWorldCharacter>(SelectedActor);

	if (!NewUnit)
	{
		if (OldUnit)
		{
			OldUnit->Select(false);
		}

		SelectedActor = nullptr;
		return;
	}

	if (OldUnit && OldUnit != NewUnit)
	{
		OldUnit->Select(false);
	}

	if (OldUnit == NewUnit)
	{
		return;
	}

	SelectedActor = NewUnit;
	NewUnit->Select(true);
}

void AUnrealWorldPlayerController::OnPrimaryReleased()
{
}

void AUnrealWorldPlayerController::OnSecondaryInputStarted()
{
	AUnrealWorldCharacter* Unit = Cast<AUnrealWorldCharacter>(SelectedActor);
	if(Unit)
	{
		Unit->GetController()->StopMovement();
	}
}

// Triggered every frame when the input is held down
void AUnrealWorldPlayerController::OnSecondaryTriggered()
{
    FollowTime += GetWorld()->GetDeltaSeconds();

    FHitResult Hit;
    const bool bHitSuccessful =
        GetHitResultUnderCursor(ECC_Visibility, true, Hit);

    if (!bHitSuccessful)
    {
        return;
    }

    CachedDestination = Hit.Location;
    AUnrealWorldCharacter* Unit = Cast<AUnrealWorldCharacter>(SelectedActor);

    if (Unit)
    {
        const FVector WorldDirection =
            (CachedDestination - Unit->GetActorLocation()).GetSafeNormal();

        Unit->AddMovementInput(WorldDirection, 1.0f, false);
    }
	
}

void AUnrealWorldPlayerController::OnSecondaryReleased()
{
	if (FollowTime <= ShortPressThreshold)
	{
		AUnrealWorldCharacter* Unit = Cast<AUnrealWorldCharacter>(SelectedActor);

		FHitResult Hit;
		const bool bHitSuccessful = GetHitResultUnderCursor(ECC_Visibility, true, Hit);

		if (bHitSuccessful)
		{
			CachedDestination = Hit.Location;
		}

		if (Unit)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(Unit->GetController(), CachedDestination);
		}

		if (FXCursor)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				FXCursor,
				CachedDestination,
				FRotator::ZeroRotator,
				FVector(1.f, 1.f, 1.f),
				true,
				true,
				ENCPoolMethod::None,
				true
			);

			UE_LOG(LogTemplateCharacter, Warning, TEXT("Spawned Niagara at %s"), *CachedDestination.ToString());
		}
		else
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("FXCursor is null"));
		}
	}

	FollowTime = 0.f;
}

void AUnrealWorldPlayerController::OnPanTriggered()
{
	float MouseX, MouseY;
	if (!GetMousePosition(MouseX, MouseY))
	{
		return;
	}

	const FVector2D CurrentMousePosition(MouseX, MouseY);

	if (!bIsPanning)
	{
		bIsPanning = true;
		LastPanMousePosition = CurrentMousePosition;
		return;
	}

	const FVector2D MouseDelta = CurrentMousePosition - LastPanMousePosition;
	LastPanMousePosition = CurrentMousePosition;
	
	if (ControlledPawn)
	{
		FVector Right = ControlledPawn->GetActorRightVector();
		FVector Forward = ControlledPawn->GetActorUpVector();

		FVector PanOffset = (-Right * MouseDelta.X + -Forward * -MouseDelta.Y) * PanSpeed;
		
		//Print mouse pos.
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				1,                  // Same key: replaces prior message each frame
				0.0f,               // Lifetime does not matter much because it refreshes
				FColor::Yellow,
				FString::Printf(TEXT("PanOffset: %s"), *PanOffset.ToString())
			);
		}
		
		PanOffset.Z = 0.0f;
		ControlledPawn->AddActorWorldOffset(PanOffset, true);
	}
}

void AUnrealWorldPlayerController::OnPanReleased()
{
	bIsPanning = false;
}

void AUnrealWorldPlayerController::ZoomIn()
{
	ControlledPawn->AddActorWorldOffset(FVector(0, 0, 50));
}

void AUnrealWorldPlayerController::ZoomOut()
{
	
	if (ControlledPawn->GetActorLocation().Z > -650)
	ControlledPawn->AddActorWorldOffset(FVector(0, 0, -50));
}
