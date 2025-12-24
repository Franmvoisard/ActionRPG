// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "FCooldownInteractable.h"
#include "FHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AFHealthPotion : public AFCooldownInteractable
{
	GENERATED_BODY()
	AFHealthPotion();
	
protected:
	bool HealInstigator(APawn* InstigatorToHeal);
	UPROPERTY(EditAnywhere, Category = "Potion")
	int32 CreditCost;


public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual FText GetInteractionText_Implementation(APawn* InstigatorPawn) override;
};