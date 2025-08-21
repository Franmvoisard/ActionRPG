// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "FCooldownInteractable.h"
#include "FCoinPile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AFCoinPile : public AFCooldownInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFCoinPile();

protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
	
