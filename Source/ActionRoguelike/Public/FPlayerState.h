// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "FGameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "FPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AFPlayerState : public APlayerState
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AFPlayerState*, PlayerState, int, NewCredits, int, Amount);
	GENERATED_BODY()
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_Credits, EditDefaultsOnly, Category = "Credits")
	int Credits;

	UFUNCTION()
	void OnRep_Credits(int OldCredits);
	
public:
	AFPlayerState();

	UPROPERTY(BlueprintAssignable, Category = "Credits")
	FOnCreditsChanged OnCreditsChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Credits")
	int GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int Amount);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool SpendCredits(int Amount);

	UFUNCTION(BlueprintNativeEvent, Category = "Save System")
	void SavePlayerState(UFSaveGame* SaveObject);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Save System")
	void LoadPlayerState(UFSaveGame* SaveObject);
	
};
