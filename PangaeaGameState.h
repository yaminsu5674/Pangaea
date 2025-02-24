// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PangaeaGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerChangedDelegate, float, Timer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameWinLoseDelegate, bool, Win);



UCLASS()
class PANGAEA_API APangaeaGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnTimerChanged, Category = "Pangaea")
	float Timer = 0;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnGameWin, Category = "Pangaea")
	bool GameWin;

	UPROPERTY(BlueprintAssignable, Category = "Pangaea")
	FOnTimerChangedDelegate OnTimerChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Pangaea")
	FOnGameWinLoseDelegate OnGameWinLoseDelegate;



public:
	void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Pangaea")
	void OnTimerChanged();

	UFUNCTION(BlueprintCallable, Category = "Pangaea")
	void OnGameWin();

	UFUNCTION(BlueprintCallable, Category = "Pangaea")
	void OnGameLose();



	
};
