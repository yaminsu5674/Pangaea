// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangaeaPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "PangaeaCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "PlayerAvatar.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

APangaeaPlayerController::APangaeaPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void APangaeaPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void APangaeaPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("Attack", IE_Pressed, this, &APangaeaPlayerController::OnAttackPressed);
	InputComponent->BindAxis("MoveForward", this, &APangaeaPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APangaeaPlayerController::MoveRight);

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &APangaeaPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &APangaeaPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &APangaeaPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &APangaeaPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &APangaeaPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &APangaeaPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &APangaeaPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &APangaeaPlayerController::OnTouchReleased);

	
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}


}

void APangaeaPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void APangaeaPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void APangaeaPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void APangaeaPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void APangaeaPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}


void APangaeaPlayerController::OnAttackPressed()
{
	auto playerAvatar = Cast<APlayerAvatar>(GetPawn());
	if (playerAvatar != nullptr && playerAvatar->CanAttack())
	{
		StopMovement();
		playerAvatar->Attack_RPC();
	}
}

void APangaeaPlayerController::MoveForward(float value)
{
	if (value != 0.0f)
	{
		// Y축 방향으로 이동 (앞/뒤)
		APawn* MyPawn = GetPawn();
		if (MyPawn)
		{
			// 월드 좌표를 기준으로 입력값에 따라 이동
			FVector ForwardDirection = FVector(1.0f, 0.0f, 0.0f); // X축 방향
			MyPawn->AddMovementInput(ForwardDirection, value);
		}
	}
}

void APangaeaPlayerController::MoveRight(float value)
{
	if (value != 0.0f)
	{
		// X축 방향으로 이동 (왼쪽/오른쪽)
		APawn* MyPawn = GetPawn();
		if (MyPawn)
		{
			// 월드 좌표를 기준으로 입력값에 따라 이동
			FVector RightDirection = FVector(0.0f, 1.0f, 0.0f); // Y축 방향
			MyPawn->AddMovementInput(RightDirection, value);
		}
	}
}
