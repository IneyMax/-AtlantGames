// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Player/AGPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"


void AAGPlayerState::IncreaseScore_Implementation(const int32 InAddScore)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(AAGPlayerState, CurrentScore, this);
	CurrentScore += InAddScore;
	if (!IsRunningDedicatedServer())
	{
		OnRep_Score();
	}
}

void AAGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.RepNotifyCondition = REPNOTIFY_Always;

	DOREPLIFETIME_WITH_PARAMS_FAST(AAGPlayerState, CurrentScore, SharedParams);
}

void AAGPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	OnScoreUpdate.Broadcast(this, GetCurrentScore());
}
