// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PangaeaCharacter.h"
#include "Weapon.h"
#include "PlayerAvatar.generated.h"

UCLASS(Blueprintable)
class PANGAEA_API APlayerAvatar : public APangaeaCharacter
{
	GENERATED_BODY()
public:
	APlayerAvatar();


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
		Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* _SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
		Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* _CameraComponent;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Pangaea|PlayerAvatar")
	void AttachWeapon(AWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Pangaea|PlayerAvatar")
	void DropWeapon();


	UFUNCTION(Server, Reliable)
	void Attack_RPC();

	void Hit(int damage) override;

	void DieProcess() override;


	FORCEINLINE
		class UCameraComponent* GetCameraComponent() const
	{
		return _CameraComponent;
	}

	FORCEINLINE
		class USpringArmComponent* GetStringArmComponent() const
	{
		return _SpringArmComponent;
	}



protected:
	virtual void BeginPlay() override;
};