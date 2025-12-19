// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "FPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AFPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;
	
	UPROPERTY()
	UUserWidget* PauseMenuInstance;
	
	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UInputAction* PauseInputAction;
	
	virtual void SetupInputComponent() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BeginPlayingStateEvent();
	
	virtual void BeginPlayingState() override;
};
