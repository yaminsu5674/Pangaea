// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "PlayerAvatar.h"
#include "PangaeaGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(_MeshComponent);

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	_PangaeaGameMode = Cast<APangaeaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Reset();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() == NM_Client)
	{
		return;
	}

	if (_LifeCountingDown > 0.0f)
	{
		FVector currentLocation = GetActorLocation();
		FVector vel = GetActorRotation().RotateVector(FVector::ForwardVector)
			* Speed * DeltaTime;
		FVector nextLocation = currentLocation + vel;
		SetActorLocation(nextLocation);

		// 래이 캐스팅 
		FHitResult hitResult;
		FCollisionObjectQueryParams objCollisionQueryParams;
		objCollisionQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

		if (GetWorld()->LineTraceSingleByObjectType(hitResult,
			currentLocation,
			nextLocation,
			objCollisionQueryParams))
		{
			auto playerAvatar = Cast<APlayerAvatar>(hitResult.GetActor());
			if (playerAvatar != nullptr)
			{
				playerAvatar->Hit(Damage);
				PrimaryActorTick.bCanEverTick = false;
				Destroy();
			}
		}

		_LifeCountingDown -= DeltaTime;
	}
	else
	{
		/*PrimaryActorTick.bCanEverTick = false;
		Destroy();*/

		_PangaeaGameMode->RecycleFireball(this);
	}
}


void AProjectile::Reset()
{
	_LifeCountingDown = Lifespan;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}