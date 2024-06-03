// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AGPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitPlayerState, APlayerState*, InPlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameHasEnded, AActor*, EndGameFocus, bool, IsWinner);


UCLASS()
class ATLANTGAMES_API AAGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnInitPlayerState OnInitPlayerState;

	UPROPERTY(BlueprintAssignable)
	FOnGameHasEnded OnGameHasEnded;

public:
	virtual void BeginPlay() override;
	virtual void InitPlayerState() override;
	virtual void OnRep_PlayerState() override;
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;

public:
	UFUNCTION(Client, Reliable)
	void GameHasEnded_Client(AActor* EndGameFocus, bool bIsWinner);
};
