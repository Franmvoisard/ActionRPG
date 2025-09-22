// Public Domain - 2025 Franco Voisard.
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.


#include "FCoinPile.h"
#include "FPlayerState.h"
#include "Kismet/GameplayStatics.h"

class UFAttributeComponent;
class AFPlayerState;

AFCoinPile::AFCoinPile()
{
}

void AFCoinPile::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	ensure(IsValid(InstigatorPawn));
	if (!HasAuthority()) return;
	if (IsInteractable)
	{
		AFPlayerState* PlayerState = InstigatorPawn->GetPlayerState<AFPlayerState>();
		
		if (PlayerState)
		{
			PlayerState->AddCredits(5);
			GetWorldTimerManager().SetTimer(Timer_ResetInteraction, this, &AFCoinPile::ResetInteractionTimer_Elapsed,Cooldown);
			SetInteractionState(false);
		}
	}	
}

