// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AGGameMode.h"

#include "EngineUtils.h"
#include "Core/AGGameState.h"
#include "Core/Player/AGPlayerState.h"
#include "Engine/PlayerStartPIE.h"
#include "GameFramework/PlayerStart.h"
#include "GameObject/Pickup/PickupBase.h"
#include "GameObject/Pickup/PickupSpawnTargetPoint.h"
#include "GameObject/Pickup/PickupTypeDataAsset.h"
#include "UObject/ObjectSaveContext.h"

DEFINE_LOG_CATEGORY(LogAGGameMode);


AAGGameMode::AAGGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentGameSessionTime = TNumericLimits<float>().Max();
}

#if WITH_EDITOR
/*
 * Remove empty items
 */
void AAGGameMode::PreSave(FObjectPreSaveContext SaveContext)
{
	for (auto It = AllowedPickups.CreateIterator(); It; ++It)
	{
		if (It->IsNull())
		{
			It.RemoveCurrent();
		}
	}
	UObject::PreSave(SaveContext);
}
#endif

void AAGGameMode::StartMatch()
{
	Super::StartMatch();

	int32 SelectedSeed = RandomSeed;
	if (bUseRandomSeed)
	{
		SelectedSeed = GetRandomSeed();
	}
	MainRandomStream.Initialize(SelectedSeed);
	SelectPurposePickupType();
	GeneratePickups();
	
	FMatchPurpose MatchPurpose;
	MatchPurpose.PurposePickupType = PurposePickupType;
	MatchPurpose.NumPurposePickup = NumPurposePickup;
	if (const auto CurrentGameState = GetGameState<AAGGameState>())
	{
		CurrentGameState->SetMatchPurpose(MatchPurpose);
		CurrentGameState->SetGameSessionTime(GameSessionTime);
		CurrentGameState->OnTotalScoreUpdate.AddUniqueDynamic(this, &AAGGameMode::OnTotalScoreUpdate);
		CurrentGameState->OnSessionTimeUpdate.AddUniqueDynamic(this, &AAGGameMode::OnSessionTimeUpdate);
	}
	
}

void AAGGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	GetGameInstance()->GetTimerManager().SetTimer(MatchEndedTimerHandle, this, &AGameMode::StartToLeaveMap, DelayBeforeLeaveMap, false);

	TObjectPtr<AActor> EndGameFocus = nullptr;
	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		if ((*It)->ActorHasTag(EndMatchViewTargetTag))
		{
			EndGameFocus = *It;
		}
	}
	
	for( FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator )
	{
		if (APlayerController* PC = Iterator->Get())
		{
			PC->GetPawn()->DisableInput(PC);
			PC->GameHasEnded(EndGameFocus, PC->GetPlayerState<AAGPlayerState>()->GetCurrentScore() >= NumPurposePickup);
		}
	}
}

void AAGGameMode::HandleLeavingMap()
{
	Super::HandleLeavingMap();

	for (auto Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
	{
		AController* Controller = Iterator->Get();
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			// We simply destroy the controllers, because there is no main menu, lobby or restart
			PlayerController->Destroyed();
		}
	}
}

AActor* AAGGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	APlayerStart* FoundPlayerStart = nullptr;
	const UClass* PawnClass = GetDefaultPawnClassForController(Player);
	const APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>() : nullptr;
	UWorld* World = GetWorld();
	TArray<APlayerStart*> BlockingGeometryStartPoints;
	TArray<APlayerStart*> UnBlockingGeometryStartPoints;
	
	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		APlayerStart* PlayerStart = *It;
		if (PlayerStart->IsA<APlayerStartPIE>())
		{
			FoundPlayerStart = PlayerStart;
			break;
		}
		
		FVector ActorLocation = PlayerStart->GetActorLocation();
		const FRotator ActorRotation = PlayerStart->GetActorRotation();
		if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
		{
			if (!OccupiedPlayerStarts.Contains(PlayerStart))
			{
				UnBlockingGeometryStartPoints.Add(PlayerStart);
			}
		}
		else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
		{
			if (!OccupiedPlayerStarts.Contains(PlayerStart))
			{
				BlockingGeometryStartPoints.Add(PlayerStart);
			}
		}
	}
	
	if (!FoundPlayerStart)
	{
		if (!UnBlockingGeometryStartPoints.IsEmpty())
		{
 			const int32 RandomStartPointIndex = MainRandomStream.RandRange(0, UnBlockingGeometryStartPoints.Num() - 1);
			FoundPlayerStart = UnBlockingGeometryStartPoints[RandomStartPointIndex];
		}
		else if (!BlockingGeometryStartPoints.IsEmpty())
		{
			const int32 RandomStartPointIndex = MainRandomStream.RandRange(0, BlockingGeometryStartPoints.Num() - 1);
			FoundPlayerStart = BlockingGeometryStartPoints[RandomStartPointIndex];
		}
	}

	if (FoundPlayerStart != nullptr)
	{
		OccupiedPlayerStarts.Add(FoundPlayerStart);
	}
	else
	{
		UE_LOG(LogAGGameMode, Display, TEXT("There are no free Player Start"));
		return Super::ChoosePlayerStart_Implementation(Player);
	}
	return FoundPlayerStart;
}

bool AAGGameMode::ReadyToStartMatch_Implementation()
{
	if (bDelayedStart)
	{
		return false;
	}
	
	if (GetMatchState() == MatchState::WaitingToStart)
	{
		if (NumPlayers + NumBots > RequiredPlayersForStart)
		{
			return true;
		}
	}
	return false;
}

void AAGGameMode::OnTotalScoreUpdate(const int32 NewTotalScore)
{
	TotalScore = NewTotalScore;
}

void AAGGameMode::OnSessionTimeUpdate(const float Time)
{
	CurrentGameSessionTime = Time;
}

void AAGGameMode::SelectPurposePickupType()
{
	const int32 RandomIndex = MainRandomStream.RandRange(0, AllowedPickups.Num() - 1);
	if (AllowedPickups.IsValidIndex(RandomIndex))
	{
		PurposePickupType = AllowedPickups[RandomIndex];
	}
}

bool AAGGameMode::GeneratePickups()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}
	
	TArray<APickupSpawnTargetPoint*> AllowedSpawnTargetPoints;
	for (TActorIterator<APickupSpawnTargetPoint> It(World); It; ++It)
	{
		if (!It->IsAllowed())
		{
			continue;
		}
		AllowedSpawnTargetPoints.Add(*It);
	}

	MainRandomStream.Reset();
	const UPickupTypeDataAsset* LoadedPickupType = PurposePickupType.LoadSynchronous();
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	int32 PurposePickupCount = 0;
	for (auto It = AllowedSpawnTargetPoints.CreateIterator(); It; ++It)
	{
		const int32 RandomSpawnIndex = MainRandomStream.UniqueRandRange(0, AllowedSpawnTargetPoints.Num() - 1);
		if (AllowedSpawnTargetPoints.IsValidIndex(RandomSpawnIndex))
		{
			const APickupSpawnTargetPoint* SpawnTargetPoint = AllowedSpawnTargetPoints[RandomSpawnIndex];
			int32 RandomIndex = 0;
			if (PurposePickupCount >= NumPurposePickup && AllowedPickups.Num() > 1)
			{
				do
				{
					RandomIndex = MainRandomStream.RandRange(0, AllowedPickups.Num() - 1);
				}
				while (AllowedPickups[RandomIndex] == PurposePickupType);
				LoadedPickupType = AllowedPickups[RandomIndex].LoadSynchronous();
			}
			else
			{
				++PurposePickupCount;
			}
			
			FTransform SpawnTransform = FTransform(SpawnTargetPoint->GetActorRotation(), SpawnTargetPoint->GetActorLocation());
			APickupBase* NewPickup = GetWorld()->SpawnActor<APickupBase>(LoadedPickupType->PickupClass, SpawnTransform, Params);
			OnSpawnNewPickup(NewPickup, AllowedPickups[RandomIndex]);
		}
	}
	return true;
}

int32 AAGGameMode::GetRandomSeed() const
{
	const FRandomStream Randomizer(static_cast<int32>(FDateTime::Now().GetTicks()));
	return Randomizer.RandRange(0, TNumericLimits<int32>().Max() - 1);
}

