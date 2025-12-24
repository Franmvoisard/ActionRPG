// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FHealthPotion.h"
#include "FAttributeComponent.h"
#include "FPlayerState.h"

class UFAttributeComponent;

AFHealthPotion::AFHealthPotion()
{
	CreditCost = 5;
}

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
			if (PlayerState && PlayerState->SpendCredits(CreditCost))
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

FText AFHealthPotion::GetInteractionText_Implementation(APawn* InstigatorPawn)
{
	UFAttributeComponent* AttributeComponent = UFAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComponent && AttributeComponent->IsFullHealth())
	{
		return NSLOCTEXT("InteractableActors", "HealthPotion_FullHealthWarning", "Already at full health.");
	}
	return FText::Format(NSLOCTEXT("InteractableActors", "HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditCost);
}
