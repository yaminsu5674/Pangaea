// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangaeaCharacter.h"
#include "PangaeaAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include <Net/UnrealNetwork.h>




APangaeaCharacter::APangaeaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void APangaeaCharacter::BeginPlay()
{
	Super::BeginPlay();
	_AnimInstance = Cast<UPangaeaAnimInstance>(GetMesh()->GetAnimInstance());
	_HealthPoints = HealthPoints;
}

void APangaeaCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

int APangaeaCharacter::GetHealthPoints()
{
	return _HealthPoints;
}

bool APangaeaCharacter::IsKilled()
{
	return (_HealthPoints <= 0.0f);
}

bool APangaeaCharacter::CanAttack()
{
	return (_AttackCountingDown <= 0.0f &&
		_AnimInstance->State == ECharacterState::Locomotion);
}

void APangaeaCharacter::Attack()
{
	_AttackCountingDown = AttackInterval;
}

bool APangaeaCharacter::IsAttacking()
{
	return (_AnimInstance->State == ECharacterState::Attack);
}

void APangaeaCharacter::Hit(int damage)
{
	if (IsKilled())
	{
		return;
	}

	if ((GetNetMode() == ENetMode::NM_ListenServer && HasAuthority()) || GetNetMode() == ENetMode::NM_Standalone)
	{
		_HealthPoints -= damage;
		OnHealthPointsChanged();
	}
	
}

void APangaeaCharacter::DieProcess()
{
	PrimaryActorTick.bCanEverTick = false;
	Destroy();
	GEngine->ForceGarbageCollection(true);
}

void APangaeaCharacter::Attack_Broadcast_RPC_Implementation()
{
	Attack();
}

void APangaeaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APangaeaCharacter, _HealthPoints);
}


void APangaeaCharacter::OnHealthPointsChanged()
{
	if (HealthBarWidget != nullptr)
	{
		float normalizedHealth = FMath::Clamp(
			(float)_HealthPoints / HealthPoints, 0.0f, 1.0f);
		auto healthBar = Cast<UHealthBarWidget>(HealthBarWidget);
		healthBar->HealthProgressBar->SetPercent(normalizedHealth);
	}

	if (_AnimInstance != nullptr)
	{
		_AnimInstance->State = ECharacterState::Hit;
	}

	if (IsKilled())
	{
		PrimaryActorTick.bCanEverTick = false;
	}
}