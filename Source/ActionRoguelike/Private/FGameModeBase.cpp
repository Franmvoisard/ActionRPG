// Public Domain - 2025 Franco Voisard.
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.


#include "FGameModeBase.h"

#include "DebugCVar.h"
#include "EngineUtils.h"
#include "FActionComponent.h"
#include "FCharacter.h"
#include "FCoinPile.h"
#include "FPlayerState.h"
#include "FSaveGame.h"
#include "AI/FAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

AFGameModeBase::AFGameModeBase()
{
	InteractablesLocation = TArray<FVector>();
	SpawnTimerInterval = 2.0f;
	PlayerRespawnDelay = 2.2f;
	CreditsPerKill = 1;
	SlotName = "SaveGame01";
}

void AFGameModeBase::StartPlay()
{
	Super::StartPlay();
	LoadSaveGame();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AFGameModeBase::SpawnBotTimer_Elapsed, SpawnTimerInterval, true);
	SpawnInteractables();
}

void AFGameModeBase::SpawnBotTimer_Elapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AFGameModeBase::OnSpawnBotQueryCompleted);
}

void AFGameModeBase::SpawnInteractables()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnInteractablesQuery, this, EEnvQueryRunMode::RandomBest25Pct, nullptr);
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AFGameModeBase::OnSpawnInteractablesQueryCompleted);
}

void AFGameModeBase::KillAllBots()
{
	for (TActorIterator<AFAICharacter> It(GetWorld()); It; ++It)
	{
		AFAICharacter* Bot = *It;
		if (UFAttributeComponent* AttributeComponent = UFAttributeComponent::GetAttributes(Bot))
		{
			if (AttributeComponent->IsAlive())
			{
				AttributeComponent->Kill(this);
			}	
		}
	}
}

void AFGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (!DebugCVar::IsBotSpawningEnabled())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bots spawn disabled via cvar 'CVarSpawnBots'"));
		return;
	}
	
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Failed!"));
		return;
	}

	int NumberOfAliveBots = 0;

	for (TActorIterator<AFAICharacter> It(GetWorld()); It; ++It)
	{
		AFAICharacter* Bot = *It;
		if (UFAttributeComponent::IsActorAlive(Bot))
		{
			NumberOfAliveBots++;
		}
	}
	int MaxBotCount = 10;
	
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
	
	if (NumberOfAliveBots >= MaxBotCount) return;
	
	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	if (SpawnLocations.Num() > 0)
	{
		if (MonsterTable)
		{
			TArray<FMonsterTypeInfoRow*> MonsterTypesRows;
			MonsterTable->GetAllRows("", MonsterTypesRows);
			
			int32 RandomIndex = FMath::RandRange(0, MonsterTypesRows.Num() - 1);
			FMonsterTypeInfoRow* MonsterType = MonsterTypesRows[RandomIndex];

			if (UAssetManager* Manager = UAssetManager::GetIfInitialized())
			{
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AFGameModeBase::OnMonsterLoaded, MonsterType->MonsterId, SpawnLocations[0]);
				Manager->LoadPrimaryAsset(MonsterType->MonsterId, Bundles, Delegate);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Asset Manager not found."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No monster table found!"));
		}
	}
}

void AFGameModeBase::OnSpawnInteractablesQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Interactables EQS Failed!"));
		return;
	}
	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	if (SpawnLocations.Num() > 0)
	{
		for (size_t i = 0; i < SpawnLocations.Num(); i++)
		{
			int RandomIndex = FMath::RandRange(0, InteractablesClasses.Num() - 1);
			AFCooldownInteractable* SpawnedObject = GetWorld()->SpawnActor<AFCooldownInteractable>(InteractablesClasses[RandomIndex], SpawnLocations[i], FRotator::ZeroRotator);
			InteractablesLocation.Add(SpawnedObject->GetActorLocation());
			if (InteractablesLocation.Num() == 10) break;
		}

		if (InteractablesLocation.Num() < 10)
		{
			SpawnInteractables();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No spawn locations found for coin EQS!"));
	}
	
}

void AFGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	AFCharacter* Player = Cast<AFCharacter>(Victim);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate_RespawnDelay;
		Delegate_RespawnDelay.BindUObject(this, &AFGameModeBase::RespawnPlayerElapsed, Player->GetController());
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate_RespawnDelay, PlayerRespawnDelay, false);
	}
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		AFPlayerState* PlayerState = KillerPawn->GetPlayerState<AFPlayerState>();
		if (PlayerState)
		{
			PlayerState->AddCredits(CreditsPerKill);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Player %s killed by %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));
}

void AFGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void AFGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AFGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
    UAssetManager* Manager = UAssetManager::GetIfInitialized();
    if (!Manager)
    {
        UE_LOG(LogTemp, Error, TEXT("Asset Manager not initialized while loading monster %s"), *LoadedId.ToString());
        return;
    }

    UFMonsterData* MonsterData = Cast<UFMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
    if (!MonsterData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get MonsterData for asset %s"), *LoadedId.ToString());
        return;
    }

    UWorld* World = GetWorld();
    if (!ensure(World))
    {
        return;
    }

    AFAICharacter* Enemy = World->SpawnActor<AFAICharacter>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
    if (!Enemy)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AI Character for %s at %s"), *LoadedId.ToString(), *SpawnLocation.ToString());
        return;
    }

    UFActionComponent* ActionComponent = Enemy->FindComponentByClass<UFActionComponent>();
    if (!ActionComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Spawned enemy %s is missing UFActionComponent"), *GetNameSafe(Enemy));
        return;
    }

    for (const TSubclassOf<UFAction>& Action : MonsterData->Actions)
    {
        if (!Action)
        {
            continue;
        }
        ActionComponent->AddAction(Enemy, Action);
    }
}


void AFGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	AFPlayerState* PlayerState = NewPlayer->GetPlayerState<AFPlayerState>();
	if (PlayerState)
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
}

void AFGameModeBase::WriteSaveGame()
{
	for (size_t i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AFPlayerState* PlayerState = Cast<AFPlayerState>(GameState->PlayerArray[i]);
		if (PlayerState)
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
			break; // Single Player only at the moment
		}
	}
	
	CurrentSaveGame->SavedActors.Empty();
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		
		if (!Actor->Implements<UFGameplayInterface>())
		{
			continue;
		}
		
		FActorSaveData ActorSaveData;
		ActorSaveData.ActorName = Actor->GetName();
		ActorSaveData.Transform = Actor->GetActorTransform();
		UE_LOG(LogTemp, Log, TEXT("Saving actor %s"), *Actor->GetName());
		
		//Convert to binary and write to the actor's binary data
		FMemoryWriter MemWriter(ActorSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);
		
		CurrentSaveGame->SavedActors.Add(ActorSaveData);
	}
	
	
	if (UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0))
	{
		UE_LOG(LogTemp, Log, TEXT("Saved save game to slot %s"), *SlotName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to save save game to slot %s"), *SlotName);
	}
}

void AFGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UFSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load save game from slot %s"), *SlotName);
			return;
		}
		
		UE_LOG(LogTemp, Log, TEXT("Loaded save game from slot %s"), *SlotName);
		
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			
			if (!Actor->Implements<UFGameplayInterface>())
			{
				continue;
			}
			
			for (FActorSaveData& ActorSaveData : CurrentSaveGame->SavedActors)
			{
				if (ActorSaveData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorSaveData.Transform);
					FMemoryReader MemReader(ActorSaveData.ByteData);
					UE_LOG(LogTemp, Log, TEXT("Loaded actor %s"), *Actor->GetName());
					UE_LOG(LogTemp, Log, TEXT("Loaded actor transform %s"), *Actor->GetTransform().ToHumanReadableString());
					//Convert binary data into asset
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					Actor->Serialize(Ar);
					IFGameplayInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UFSaveGame>(UGameplayStatics::CreateSaveGameObject(UFSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created new save game in slot %s"), *SlotName);
	}
	
	
}
