// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGTypes.h"
#include "GameFramework/GameState.h"
#include "AGGameState.generated.h"

class AAGPlayerState;
class UPickupTypeDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetMatchPurposeType, FMatchPurpose, PickupType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerScoreUpdate, AAGPlayerState*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTotalScoreUpdate, int32, NewTotalScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionTimeUpdate, float, Time);



UCLASS(Blueprintable)
class ATLANTGAMES_API AAGGameState : public AGameState
{
	GENERATED_BODY()

public:
	AAGGameState(const FObjectInitializer& ObjectInitializer);
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnSetMatchPurposeType OnSetMatchPurposeType;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerScoreUpdate OnPlayerScoreUpdate;
		
	UPROPERTY(BlueprintAssignable)
	FOnTotalScoreUpdate OnTotalScoreUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnSessionTimeUpdate OnSessionTimeUpdate;
	
public:
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void HandleMatchHasEnded() override;
	
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void PlayerScoreUpdate_Multicast(AAGPlayerState* InPlayerState, int32 InAddScore);
	
	UFUNCTION(BlueprintPure)
    FMatchPurpose GetMatchPurpose() const;
	
	void SetMatchPurpose(const FMatchPurpose& InPurpose);
	void SetGameSessionTime(float InGameSessionTime);

protected:
	UFUNCTION()
	void OnRep_Purpose();

	UFUNCTION()
	void OnRep_GameSessionTime();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_Purpose)
	FMatchPurpose Purpose;

	UPROPERTY(ReplicatedUsing=OnRep_GameSessionTime)
	float GameSessionTime = 15;

	UPROPERTY(EditDefaultsOnly)
	float SessionTimeUpdateRate = 1.f;
	
	UPROPERTY()
	int32 TotalScore = 0;

private:
	float AccumulateDeltaSeconds = 0.f;
};

FORCEINLINE FMatchPurpose AAGGameState::GetMatchPurpose() const
{
	return Purpose;
}
