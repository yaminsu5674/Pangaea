// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "PlayerAvatar.h"
#include "PangaeaCharacter.h"
#include "DefenseTower.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(_StaticMesh);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this,
		&AWeapon::OnWeaponBeginOverlap);

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Holder == nullptr)
	{
		FQuat rotQuat = FQuat(
			FRotator(0, 300.0f * DeltaTime, 0)
		);
		AddActorLocalRotation(rotQuat);
	}
}

void AWeapon::OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	auto character = Cast<APangaeaCharacter>(OtherActor);

	if (OtherActor)
	{
		FString ClassName = OtherActor->GetClass()->GetName();
		FString OverlapName = OverlappedActor->GetClass()->GetName();
	}

	if (character != nullptr)
	{
		if (Holder == nullptr)
		{
			auto playerAvatar = Cast<APlayerAvatar>(character);
			if (playerAvatar != nullptr)
			{
				Holder = character;
				playerAvatar->DropWeapon();
				playerAvatar->AttachWeapon(this);
			}
		}
		else if (character != Holder &&
			IsWithinAttackRange(500.0f, OtherActor) &&
			character->CanBeDamaged() &&
			Holder->IsAttacking())
		{
			character->Hit(Holder->Strength);
		}

	}
	
}


bool AWeapon::IsWithinAttackRange(float AttackRange, AActor* Target)
{
	return (AttackRange <= 0.0f || FVector::Distance(Target->GetActorLocation(), GetActorLocation()) <= AttackRange);
}