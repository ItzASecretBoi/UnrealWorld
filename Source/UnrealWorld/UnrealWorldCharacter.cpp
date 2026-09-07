// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"


void AUnrealWorldCharacter::BeginPlay()
{
	Super::BeginPlay();

	SelectionMesh = FindComponentByClass<UStaticMeshComponent>();
	
	
	if (SelectionMesh)
	{
		SelectionMesh->SetHiddenInGame(true);
	}
}

AUnrealWorldCharacter::AUnrealWorldCharacter()
{
	
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	SelectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionMesh"));
	SelectionMesh->SetupAttachment(GetRootComponent());

	SelectionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SelectionMesh->SetGenerateOverlapEvents(false);
	SelectionMesh->SetHiddenInGame(true);
	SelectionMesh->SetVisibility(true);
	SelectionMesh->SetCastShadow(false);
	SelectionMesh->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	SelectionMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	SelectionMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	
}

void AUnrealWorldCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

UStaticMeshComponent* AUnrealWorldCharacter::GetSelectionMesh() const
{
	return SelectionMesh;
}

void AUnrealWorldCharacter::Select(bool Show)
{
	SelectionMesh->SetHiddenInGame(!Show);
	SelectionMesh->SetVisibility(Show);
}
