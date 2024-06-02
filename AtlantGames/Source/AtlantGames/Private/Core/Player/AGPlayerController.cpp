// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Player/AGPlayerController.h"


void AAGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	OnInitPlayerState.Broadcast(PlayerState);
}

void AAGPlayerController::InitPlayerState()
{
	Super::InitPlayerState();

	OnInitPlayerState.Broadcast(PlayerState);
}

void AAGPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnInitPlayerState.Broadcast(PlayerState);
}

void AAGPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	GameHasEnded_Client(EndGameFocus, bIsWinner);
}

void AAGPlayerController::GameHasEnded_Client_Implementation(AActor* EndGameFocus, const bool bIsWinner)
{
	GetPawn()->DisableInput(this);
 	OnGameHasEnded.Broadcast(EndGameFocus, bIsWinner);
}
