// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGTypes.h"
#include "Engine/DataAsset.h"
#include "PickupTypeDataAsset.generated.h"

class APickupBase;


UCLASS(BlueprintType)
class ATLANTGAMES_API UPickupTypeDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APickupBase> PickupClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPickupData PickupData;
};


