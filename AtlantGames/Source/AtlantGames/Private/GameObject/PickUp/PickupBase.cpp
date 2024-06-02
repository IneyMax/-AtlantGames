// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Pickup/PickupBase.h"

#include "Components/SphereComponent.h"


APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
	SphereCollision->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	Mesh->SetupAttachment(RootComponent);
}

