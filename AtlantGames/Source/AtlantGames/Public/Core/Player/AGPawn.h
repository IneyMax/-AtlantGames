// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AGPawn.generated.h"


UCLASS()
class ATLANTGAMES_API AAGPawn : public APawn
{
	GENERATED_BODY()

public:
	AAGPawn();

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetMovementDirection(FVector2D NewMovementDirection);

	UFUNCTION(BlueprintNativeEvent)
	void K2_OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState);
	
	virtual void OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState) override;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FVector2D MovementDirection;
	
	UPROPERTY(EditDefaultsOnly)
	float Speed = 600;
};
