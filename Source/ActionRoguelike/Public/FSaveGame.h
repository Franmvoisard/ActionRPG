// Public Domain - 2025 Franco Voisard. 
// This code is provided for skill and knowledge demo purposes. 
// No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY();
	
public:
	UPROPERTY()
	FString ActorName;
	
	UPROPERTY()
	FTransform Transform;
};

UCLASS()
class ACTIONROGUELIKE_API UFSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 Credits;
	
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};
