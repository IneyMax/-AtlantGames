// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AGPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FScoreUpdate, AAGPlayerState*, PlayerState, int32, NewScore);


UCLASS(Blueprintable)
class ATLANTGAMES_API AAGPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FScoreUpdate OnScoreUpdate;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentScore() const;
	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void IncreaseScore(int32 InAddScore);

protected:
	UPROPERTY(ReplicatedUsing=OnRep_Score)
	int32 CurrentScore;
	
	virtual void OnRep_Score() override;
};

FORCEINLINE int32 AAGPlayerState::GetCurrentScore() const
{
	return CurrentScore;
}
