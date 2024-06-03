// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AGGameState.h"

#include "Core/Player/AGPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"


UE_ENABLE_OPTIMIZATION

AAGGameState::AAGGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.25f;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
}

void AAGGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.RepNotifyCondition = REPNOTIFY_Always;

	DOREPLIFETIME_WITH_PARAMS_FAST(AAGGameState, Purpose, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AAGGameState, GameSessionTime, SharedParams);
}

void AAGGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AccumulateDeltaSeconds += DeltaSeconds;
	if (AccumulateDeltaSeconds >= SessionTimeUpdateRate)
	{
		AccumulateDeltaSeconds -= SessionTimeUpdateRate;
		SetGameSessionTime(GameSessionTime - SessionTimeUpdateRate);
	}
}

void AAGGameState::SetMatchPurpose(const FMatchPurpose& InPurpose)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(AAGGameState, Purpose, this);
	Purpose = InPurpose;
	if (!IsRunningDedicatedServer())
	{
		OnRep_Purpose();
	}
}

void AAGGameState::SetGameSessionTime(const float InGameSessionTime)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(AAGGameState, Purpose, this);
	GameSessionTime = InGameSessionTime;
	if (!IsRunningDedicatedServer())
	{
		OnRep_GameSessionTime();
	}
}

void AAGGameState::PlayerIncreaseScore_Server_Implementation(AAGPlayerState* InPlayerState, const int32 InAddScore)
{
	InPlayerState->IncreaseScore(InAddScore);
	TotalScore += InAddScore;
	OnTotalScoreUpdate.Broadcast(TotalScore);
	PlayerScoreUpdate_Multicast(InPlayerState, InPlayerState->GetCurrentScore());
}

void AAGGameState::PlayerScoreUpdate_Multicast_Implementation(AAGPlayerState* InPlayerState, const int32 NewScore)
{
	OnPlayerScoreUpdate.Broadcast(InPlayerState, NewScore);
}

void AAGGameState::OnRep_Purpose()
{
	OnSetMatchPurposeType.Broadcast(Purpose);
}

void AAGGameState::OnRep_GameSessionTime()
{
	OnSessionTimeUpdate.Broadcast(GameSessionTime);
}

UE_DISABLE_OPTIMIZATION
