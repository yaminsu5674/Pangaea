// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PangaeaCharacter.generated.h"

UCLASS(Blueprintable)
class APangaeaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APangaeaCharacter();

	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	int HealthPoints = 100;

	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	float Strength = 5;

	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	float Armer = 1;

	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	float AttackRange = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	float AttackInterval = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUserWidget* HealthBarWidget;

protected:
	class UPangaeaAnimInstance* _AnimInstance;

	UPROPERTY(Replicatedusing = OnHealthPointsChanged)
	int _HealthPoints;

	float _AttackCountingDown;

public:
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimePrpos) const override;

	UFUNCTION(BlueprintCallable,
		Category = "Pangaea|Character",
		meta = (DisplayName = "Get HP"))
	int GetHealthPoints();

	UFUNCTION(BlueprintCallable, Category = "Pangaea|Character")
	bool IsKilled();

	UFUNCTION(BlueprintCallable, Category = "Pangaea|Character")
	bool CanAttack();
	bool IsAttacking();

	virtual void Attack();
	virtual void Hit(int damage);
	virtual void DieProcess();

	UFUNCTION(NetMultiCast, Reliable)
	void Attack_Broadcast_RPC();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthPointsChanged();

};