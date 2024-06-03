// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Player/AGPawn.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"


AAGPawn::AAGPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAGPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector2D VectorOffset = MovementDirection * Speed * DeltaSeconds;
	AddActorWorldOffset(FVector(VectorOffset.X, VectorOffset.Y, 0.f), true);
}

void AAGPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.RepNotifyCondition = REPNOTIFY_Always;

	DOREPLIFETIME_WITH_PARAMS_FAST(AAGPawn, MovementDirection, SharedParams);
}

void AAGPawn::SetMovementDirection_Implementation(const FVector2D NewMovementDirection)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(AAGPawn, MovementDirection, this);
	MovementDirection = NewMovementDirection;
}

void AAGPawn::K2_OnPlayerStateChanged_Implementation(APlayerState* NewPlayerState, APlayerState* OldPlayerState)
{
}

void AAGPawn::OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState)
{
	Super::OnPlayerStateChanged(NewPlayerState, OldPlayerState);

	K2_OnPlayerStateChanged(NewPlayerState, OldPlayerState);
}
