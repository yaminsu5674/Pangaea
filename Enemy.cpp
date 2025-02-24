// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyController.h"
#include "PangaeaAnimInstance.h"


DEFINE_LOG_CATEGORY_STATIC(MyLogCategory, Log, All);
/*UE_LOG(MyLogCategory, Log, TEXT("BeginPlay ran~~~~~~~~~~~~~~~~~~~~~~!!!!!!"));*/

AEnemy::AEnemy()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
	static ConstructorHelpers::FObjectFinder<UBlueprint>blueprint_finder(TEXT("Blueprint'/Game/Assets/TopDown/Blueprints/BP_Hammer.BP_Hammer'"));
	_WeaponClass = (UClass*)blueprint_finder.Object->GeneratedClass;
}																			

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	_Weapon = Cast<AWeapon>(GetWorld()->SpawnActor(_WeaponClass));
	_Weapon->Holder = this;
	_Weapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		FName("hand_rSocket"));
}

void AEnemy::Tick(float DeltaTime)
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

	if (_chasedTarget != nullptr &&
		_AnimInstance->State == ECharacterState::Locomotion)
	{
		auto enemyController = Cast<AEnemyController>(GetController());
		enemyController->MakeAttackDecision(_chasedTarget);
	}
}

void AEnemy::Chase(APawn* targetPawn)
{
	if (targetPawn != nullptr && _AnimInstance->State == ECharacterState::Locomotion)
	{
		auto enemyController = Cast<AEnemyController>(GetController());
		enemyController->MoveToActor(targetPawn, 90.0f);
	}
	_chasedTarget = targetPawn;
}

void AEnemy::DieProcess()
{
	Super::DieProcess();
	_Weapon->Destroy();
}

void AEnemy::Attack()
{
	APangaeaCharacter::Attack();

	GetController()->StopMovement();
}

void AEnemy::Hit(int damage)
{
	Super::Hit(damage);

	
}