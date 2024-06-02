// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObjectInteractionInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AGStatics.generated.h"

/**
 * 
 */
UCLASS()
class ATLANTGAMES_API UAGStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "GameObjectInteractionInterface",  meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static TScriptInterface<IGameObjectInteractionInterface> FindNearestGameObject(UObject* WorldContextObject,const FVector Origin,
		const TArray<TScriptInterface<IGameObjectInteractionInterface>>& ObjectsToCheck, float& Distance);
};
