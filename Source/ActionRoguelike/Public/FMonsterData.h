// Public Domain - 2025 Franco Voisard. 
// This code is provided for skill and knowledge demo purposes. 
// No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "FAction.h"
#include "Engine/DataAsset.h"
#include "FMonsterData.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UFMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Settings")
	TSubclassOf<AActor> MonsterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Settings")
	TArray<TSubclassOf<UFAction>> Actions;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Monsters", GetFName());
	}
};
