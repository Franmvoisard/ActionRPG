// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"

void AFPlayerController::TogglePauseMenu()
{
	// Menu is open, close it
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		bShowMouseCursor = true;
		SetInputMode( FInputModeGameOnly());
		return;
	}
	
	// Menu is closed, create it
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if(PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);
		bShowMouseCursor = true;
		SetInputMode( FInputModeUIOnly());
	}
}


void AFPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseInputAction, ETriggerEvent::Started, this, &AFPlayerController::TogglePauseMenu);
	}
}

void AFPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	BeginPlayingStateEvent();
} 
