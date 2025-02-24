// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PangaeaCharacter.h"
#include "Weapon.h"
#include "Enemy.generated.h"

UCLASS()
class PANGAEA_API AEnemy : public APangaeaCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	APawn* _chasedTarget = nullptr;
	UClass* _WeaponClass;
	AWeapon* _Weapon;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* PawnSensingComponent;


public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pangaea|Enemy")
	void Attack() override;

	void DieProcess() override;
	void Hit(int damage) override;

	UFUNCTION(BlueprintCallable, Category = "Pangaea|Enemy")
	void Chase(APawn* targetPawn);

protected:
	virtual void BeginPlay() override;

};