// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Pickup/PickupSpawnTargetPoint.h"



APickupSpawnTargetPoint::APickupSpawnTargetPoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool APickupSpawnTargetPoint::IsAllowed() const
{
	return bAllowedToSpawn;
}
