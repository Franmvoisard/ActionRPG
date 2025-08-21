// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FHealthPotion.h"

#include "FAttributeComponent.h"
#include "FPlayerState.h"
#include "Kismet/GameplayStatics.h"


class UFAttributeComponent;

AFHealthPotion::AFHealthPotion() { }

void AFHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	ensure(IsValid(InstigatorPawn));
	if (IsInteractable)
	{
		AFPlayerState* PlayerState = Cast<AFPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		if (UFAttributeComponent* InstigatorAttributeComponent = UFAttributeComponent::GetAttributes(InstigatorPawn))
		{
			if (InstigatorAttributeComponent->IsFullHealth()) return;
			if (PlayerState)
			{
				if (PlayerState->SpendCredits(5))
				{
					HealInstigator(InstigatorPawn);
					GetWorldTimerManager().SetTimer(Timer_ResetInteraction, this, &AFHealthPotion::ResetInteractionTimer_Elapsed,Cooldown);
					SetInteractionState(false);
				}
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
