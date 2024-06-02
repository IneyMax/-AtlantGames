// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameObject/GameObjectInteractionInterface.h"
#include "PickupBase.generated.h"

class USphereComponent;


UCLASS(Blueprintable)
class ATLANTGAMES_API APickupBase : public AActor, public IGameObjectInteractionInterface
{
	GENERATED_BODY()

public:
	APickupBase();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
