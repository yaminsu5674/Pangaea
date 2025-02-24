// Fill out your copyright notice in the Description page of Project Settings.


#include "PangaeaGameState.h"
#include <Net/UnrealNetwork.h>

void APangaeaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APangaeaGameState, Timer);
}

void APangaeaGameState::OnTimerChanged()
{
	OnTimerChangedDelegate.Broadcast(Timer);
}

void APangaeaGameState::OnGameWin()
{
	GameWin = true;
	OnGameWinLoseDelegate.Broadcast(GameWin);
}

void APangaeaGameState::OnGameLose()
{
	GameWin = false;
	OnGameWinLoseDelegate.Broadcast(GameWin);
}