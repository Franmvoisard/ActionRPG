// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FGameModeBase.h"

#include "EngineUtils.h"
#include "AI/FAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

AFGameModeBase::AFGameModeBase()
{
	SpawnTimerInterval = 2.0f;
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

void AFGameModeBase::OnSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Failed!"));
		return;
	}

	int NumberOfAliveBots = 0;

	for (TActorIterator<AFAICharacter> It(GetWorld()); It; ++It)
	{
		AFAICharacter* Bot = *It;
		if (UFAttributeComponent* BotAttributeComponent = Bot->GetComponentByClass<UFAttributeComponent>())
		{
			if (BotAttributeComponent->IsAlive())
			{
				NumberOfAliveBots++;
			}
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
