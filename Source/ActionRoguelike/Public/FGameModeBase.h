// Public Domain - 2025 Franco Voisard. 
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "FMonsterData.h"
#include "FSaveGame.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "FGameModeBase.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FMonsterTypeInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	FMonsterTypeInfoRow()
	{
		CreditsPerKill = 5;
		SpawnChance = 1.0f;
		SpawnCooldown = 5.0f;
	}

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	float SpawnChance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float SpawnCooldown;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int CreditsPerKill;

};
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
	

	UFUNCTION(Blueprintcallable, Category = "SaveGame")
	void WriteSaveGame();

	UFUNCTION(Blueprintcallable, Category = "SaveGame")
	void LoadSaveGame();

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
protected:
	FString SlotName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;
	
	UPROPERTY()
	UFSaveGame* CurrentSaveGame;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	int CreditsPerKill;
	
	FTimerHandle TimerHandle_SpawnBots;
	
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
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnSpawnInteractablesQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnBotTimer_Elapsed();

	UFUNCTION(Exec)
	void KillAllBots();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

private:
	void SpawnInteractables();
};
