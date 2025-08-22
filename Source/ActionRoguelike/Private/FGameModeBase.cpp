// Public Domain - 2025 Franco Voisard.
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.


#include "FGameModeBase.h"

#include "EngineUtils.h"
#include "FCharacter.h"
#include "FCoinPile.h"
#include "FPlayerState.h"
#include "AI/FAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar.SpawnBots"), true, TEXT("Enable or disable bots spawning"), ECVF_Cheat);


AFGameModeBase::AFGameModeBase()
{
	InteractablesLocation = TArray<FVector>();
	SpawnTimerInterval = 2.0f;
	PlayerRespawnDelay = 2.2f;
}

void AFGameModeBase::StartPlay()
{
	Super::StartPlay();
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

void AFGameModeBase::OnEnemyKilled(AFAICharacter* Victim, AActor* Killer)
{
	AFPlayerState* PlayerState = Cast<AFPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (PlayerState)
	{
		PlayerState->AddCredits(1);
	}
	
	Victim->OnKilled.RemoveDynamic(this, &AFGameModeBase::OnEnemyKilled);
}

void AFGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (!CVarSpawnBots.GetValueOnGameThread())
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
		AFAICharacter* Enemy = GetWorld()->SpawnActor<AFAICharacter>(MinionClass, SpawnLocations[0], FRotator::ZeroRotator);
		Enemy->OnKilled.AddDynamic(this, &AFGameModeBase::OnEnemyKilled);
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