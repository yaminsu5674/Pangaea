// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAvatar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PangaeaAnimInstance.h"
#include "PangaeaGameState.h"
#include "Kismet/GameplayStatics.h"

APlayerAvatar::APlayerAvatar()
{
	HealthPoints = 150;
	Strength = 30;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	auto characterMovement = GetCharacterMovement();
	characterMovement->bOrientRotationToMovement = true;
	characterMovement->RotationRate = FRotator(0.f, 640.f, 0.f);
	characterMovement->bConstrainToPlane = true;
	characterMovement->bSnapToPlaneAtStart = true;
	
	_SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_SpringArmComponent->SetupAttachment(RootComponent);
	_SpringArmComponent->SetUsingAbsoluteRotation(true);
	_SpringArmComponent->TargetArmLength = 800.f;
	_SpringArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	_SpringArmComponent->bDoCollisionTest = false;

	_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_CameraComponent->SetupAttachment(_SpringArmComponent, USpringArmComponent::SocketName);
	_CameraComponent->bUsePawnControlRotation = false;
}

void APlayerAvatar::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_AnimInstance->Speed = GetCharacterMovement()->Velocity.Size2D();

	if (_AttackCountingDown == AttackInterval)
	{
		_AnimInstance->State = ECharacterState::Attack;
	}

	if (_AttackCountingDown > 0.0f)
	{
		_AttackCountingDown -= DeltaTime;
	}
}

void APlayerAvatar::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerAvatar::AttachWeapon(AWeapon* Weapon)
{
	Weapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		FName("hand_rSocket"));
}

void APlayerAvatar::DropWeapon()
{
	TArray<AActor*> attachedActors;
	GetAttachedActors(attachedActors, true);
	for (int i = 0; i < attachedActors.Num(); ++i)
	{
		attachedActors[i]->DetachFromActor(
			FDetachmentTransformRules::KeepWorldTransform);
		attachedActors[i]->SetActorRotation(FQuat::Identity);
		AWeapon* weapon = Cast<AWeapon>(attachedActors[i]);
		if (weapon != nullptr)
		{
			weapon->Holder = nullptr;
		}
	}
}



void APlayerAvatar::Hit(int damage)
{
	Super::Hit(damage);

}


void APlayerAvatar::DieProcess()
{
	Super::DieProcess();

	APangaeaGameState* gameState = Cast<APangaeaGameState>(UGameplayStatics::GetGameState(GetWorld()));
	gameState->OnGameLose();
}


void APlayerAvatar::Attack_RPC_Implementation()
{
	Attack_Broadcast_RPC();
}