// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameObjectInteractionInterface.generated.h"


UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UGameObjectInteractionInterface : public UInterface
{
	GENERATED_BODY()
};


/**
 * 
 */
class ATLANTGAMES_API IGameObjectInteractionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsCanInteract(UObject* ContextObject) const;
	virtual bool IsCanInteract_Implementation(UObject* ContextObject) const { return false; }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = ( HidePin = "ContextObject", DefaultToSelf = "ContextObject"))
	void Interact(UObject* ContextObject);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetGameObjectLocation() const;
};
