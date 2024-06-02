// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "PickupSpawnTargetPoint.generated.h"


UCLASS(Blueprintable)
class ATLANTGAMES_API APickupSpawnTargetPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	APickupSpawnTargetPoint();
	bool IsAllowed() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bAllowedToSpawn = true;
};
