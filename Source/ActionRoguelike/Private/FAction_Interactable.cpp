// Public Domain - 2025 Franco Voisard.
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.


#include "FAction_Interactable.h"

#include "DebugCVar.h"
#include "FActionComponent.h"

// Sets default values
AFAction_Interactable::AFAction_Interactable()
{
	Cooldown = 0.0f;
	IsInteractable = true;
}

void AFAction_Interactable::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	if (IsInteractable)
	{
		if (UFActionComponent* ActionComponent = InstigatorPawn->GetComponentByClass<UFActionComponent>())
		{
			if (ActionComponent->GetAction(ActionToGrant))
			{			
				if (DebugCVar::IsActionsDebugEnabled())
				{
					FString DebugMessage = FString::Printf(TEXT("Action %s was already granted to %s"), *GetNameSafe(ActionToGrant), *GetNameSafe(InstigatorPawn));
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMessage);
				}
				return;
			}
			else
			{
				ActionComponent->AddAction(InstigatorPawn, ActionToGrant);
				SetInteractionState(false);
			}
		}
	}
}

