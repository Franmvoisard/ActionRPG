// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AFPlayerState : public APlayerState
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreditsChanged, int, NewCredits, int, Amount);
	GENERATED_BODY()
	
protected:
	int Credits;

public:
	AFPlayerState();

	UPROPERTY(BlueprintAssignable, Category = "PlayerState")
	FOnCreditsChanged OnCreditsChanged;
	
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	int GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void AddCredits(int Amount);

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void SpendCredits(int Amount);
};
