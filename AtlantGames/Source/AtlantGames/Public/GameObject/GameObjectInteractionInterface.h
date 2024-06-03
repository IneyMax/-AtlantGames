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
 * Т.к. в данном случае, у интерактивных объектов очень простое поведение - реализовано взаимодействие с помощью интерфейса.
 * На практике, предпочтение будет скорее за взаимодействием с помощью компонентов
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
