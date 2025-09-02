// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FHealthPotion.h"
#include "FAttributeComponent.h"
#include "FPlayerState.h"

class UFAttributeComponent;

AFHealthPotion::AFHealthPotion() { }

void AFHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	
	if (IsInteractable)
	{
		if (UFAttributeComponent* InstigatorAttributeComponent = UFAttributeComponent::GetAttributes(InstigatorPawn))
		{
			if (InstigatorAttributeComponent->IsFullHealth()) return;

			AFPlayerState* PlayerState = InstigatorPawn->GetPlayerState<AFPlayerState>();
			if (PlayerState && PlayerState->SpendCredits(5))
			{
				HealInstigator(InstigatorPawn);
				GetWorldTimerManager().SetTimer(Timer_ResetInteraction, this, &AFHealthPotion::ResetInteractionTimer_Elapsed,Cooldown);
				SetInteractionState(false);
			}
		}
	}	
}
// An object that can be either active or inactive depending on a certain condition
// Interacting with it will have an effect.

bool AFHealthPotion::HealInstigator(APawn* InstigatorToHeal)
{
	if (!InstigatorToHeal) return false;
	if (UFAttributeComponent* InstigatorAttributeComponent = UFAttributeComponent::GetAttributes(InstigatorToHeal))
	{
		return InstigatorAttributeComponent->ApplyHealthChange(this, InstigatorAttributeComponent->GetMaxHealth());
	}

	return false;
}
