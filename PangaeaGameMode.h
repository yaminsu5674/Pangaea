// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Projectile.h"
#include "PangaeaGameMode.generated.h"

UCLASS(minimalapi)
class APangaeaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APangaeaGameMode();
	~APangaeaGameMode();


protected:
	TQueue<AProjectile*, EQueueMode::Spsc> _FireballPool;

public:
	AProjectile* SpawnOrGetFireball(UClass* ProjectileClass);

	void RecycleFireball(AProjectile* projectile);
};



