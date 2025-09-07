// Public Domain - 2025 Franco Voisard.
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "FAction.h"
#include "FCooldownInteractable.h"
#include "FAction_Interactable.generated.h"

UCLASS()

class ACTIONROGUELIKE_API AFAction_Interactable : public AFCooldownInteractable
{
	GENERATED_BODY()

public:
	AFAction_Interactable();

protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TSubclassOf<UFAction> ActionToGrant;
};
