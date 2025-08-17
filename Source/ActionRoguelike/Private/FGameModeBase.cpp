// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FGameModeBase.h"

#include "EngineUtils.h"
#include "FCharacter.h"
#include "AI/FAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar.SpawnBots"), true, TEXT("Enable or disable bots spawning"), ECVF_Cheat);


AFGameModeBase::AFGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	PlayerRespawnDelay= 2.2f;
}

void AFGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AFGameModeBase::SpawnBotTimer_Elapsed, SpawnTimerInterval, true);
}

void AFGameModeBase::SpawnBotTimer_Elapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AFGameModeBase::OnSpawnQueryCompleted);
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

void AFGameModeBase::OnSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
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
		GetWorld()->SpawnActor<AActor>(MinionClass, SpawnLocations[0], FRotator::ZeroRotator);
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