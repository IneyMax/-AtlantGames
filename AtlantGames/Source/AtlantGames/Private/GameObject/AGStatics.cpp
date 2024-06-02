// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/AGStatics.h"

#include "GameObject/GameObjectInteractionInterface.h"


UE_DISABLE_OPTIMIZATION

TScriptInterface<IGameObjectInteractionInterface> UAGStatics::FindNearestGameObject(UObject* WorldContextObject,
	const FVector Origin, const TArray<TScriptInterface<
	IGameObjectInteractionInterface>>&
	ObjectsToCheck, float& Distance)
{
	TScriptInterface<IGameObjectInteractionInterface> NearestObject = nullptr;
	float DistanceFromNearestObject = Distance = TNumericLimits<float>::Max();

	for (auto ObjectToCheck : ObjectsToCheck)
	{
		const float DistanceFromObjectToCheck =
			(Origin - ObjectToCheck->Execute_GetGameObjectLocation(ObjectToCheck.GetObject())).SizeSquared();
		if (DistanceFromObjectToCheck < DistanceFromNearestObject)
		{
			NearestObject = ObjectToCheck;
			DistanceFromNearestObject = DistanceFromObjectToCheck;
		}
	}

	if (NearestObject)
	{
		Distance = FMath::Sqrt(DistanceFromNearestObject);
	}

	return NearestObject;
}

UE_ENABLE_OPTIMIZATION
