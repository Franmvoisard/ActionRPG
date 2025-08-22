// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "AI/FAICharacter.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "FGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AFGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFGameModeBase();
	virtual void StartPlay() override;
	virtual void OnActorKilled(AActor* Victim, AActor* Killer);

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TArray<FVector> InteractablesLocation;
	
protected:
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AFAICharacter> MinionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TArray<TSubclassOf<AActor>> InteractablesClasses;
	
	UPROPERTY(EditDefaultsOnly, Category= "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Coins")
	UEnvQuery* SpawnInteractablesQuery;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float PlayerRespawnDelay;
	
	UFUNCTION()
	void OnEnemyKilled(AFAICharacter* Victim, AActor* Killer);
	
	UFUNCTION()
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnSpawnInteractablesQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	UFUNCTION()
	void SpawnBotTimer_Elapsed();

	UFUNCTION(Exec)
	void KillAllBots();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

private:
	void SpawnInteractables();
};
