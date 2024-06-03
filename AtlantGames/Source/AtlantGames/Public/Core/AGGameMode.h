// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGTypes.h"
#include "GameFramework/GameMode.h"
#include "GameObject/Pickup/PickupBase.h"
#include "AGGameMode.generated.h"

class UPickupTypeDataAsset;
class APlayerStart;

DECLARE_LOG_CATEGORY_EXTERN(LogAGGameMode, Log, All);


UCLASS()
class ATLANTGAMES_API AAGGameMode : public AGameMode
{
	GENERATED_BODY()

	AAGGameMode(const FObjectInitializer& ObjectInitializer);

public:
#if WITH_EDITOR
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
	
	virtual void StartMatch() override;
	virtual void HandleMatchHasEnded() override;
	virtual void HandleLeavingMap() override;
	/* Override for preventing spawn at 1 start point*/
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual bool ReadyToStartMatch_Implementation() override;
	virtual bool ReadyToEndMatch_Implementation() override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnSpawnNewPickup(APickupBase* NewPickup, const TSoftObjectPtr<UPickupTypeDataAsset>& InPickupData);

	UFUNCTION()
	void OnTotalScoreUpdate(int32 NewTotalScore);

	UFUNCTION()
	void OnSessionTimeUpdate(float Time);
	
	void SelectPurposePickupType();
	bool GeneratePickups();
	int32 GetRandomSeed() const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<UPickupTypeDataAsset>> AllowedPickups;
	
	UPROPERTY(EditDefaultsOnly)
	bool bUseRandomSeed = false;
	
	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="!bUseRandomSeed"))
    int32 RandomSeed = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 RequiredPlayersForStart = 2;

	UPROPERTY(EditDefaultsOnly)
	int32 NumPurposePickup = 5;
	
	UPROPERTY(EditDefaultsOnly)
	float DelayBeforeLeaveMap = 10.f;

	UPROPERTY(EditDefaultsOnly)
	FName EndMatchViewTargetTag = "EndMatchViewTarget";

	UPROPERTY(EditDefaultsOnly)
	float GameSessionTime = 30.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UPickupTypeDataAsset> PurposePickupType;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<APlayerStart>> OccupiedPlayerStarts;
	
	FAGRandomizer MainRandomStream;
	FTimerHandle MatchEndedTimerHandle;
	float CurrentGameSessionTime;
	int32 TotalScore = 0.f;
};

FORCEINLINE bool AAGGameMode::ReadyToEndMatch_Implementation()
{
	return TotalScore >= NumPurposePickup || CurrentGameSessionTime <= 0;
}