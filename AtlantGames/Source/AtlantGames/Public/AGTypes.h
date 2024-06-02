// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AGTypes.generated.h"

class UPickupTypeDataAsset;
class UStaticMesh;


USTRUCT(BlueprintType, Atomic)
struct ATLANTGAMES_API FMatchPurpose
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TSoftObjectPtr<UPickupTypeDataAsset> PurposePickupType;

	UPROPERTY(BlueprintReadWrite)
	int32 NumPurposePickup = 0;
};


USTRUCT(BlueprintType)
struct ATLANTGAMES_API FPickupData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor Color = FLinearColor::White;
};


USTRUCT(BlueprintType)
struct FAGRandomizer : public FRandomStream
{
	GENERATED_BODY()
	
	TSet<int32> UsedIndexes;

	int32 UniqueRandRange(const int32 Min, const int32 Max)
	{
		if (UsedIndexes.Num() - 1 >= Max)
		{
			ensureMsgf(false, TEXT("Not available unique index"));
			return -1;
		}
		
		const int32 RandInt = RandRange(Min, Max);
		if (!UsedIndexes.Contains(RandInt))
		{
			UsedIndexes.Add(RandInt);
			return RandInt;
		}
		
		return UniqueRandRange(Min, Max);
	}

	void Reset()
	{
		UsedIndexes.Empty();
	}
};
