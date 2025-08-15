// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UFInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UFInteractionComponent();
	void PrimaryInteract();

protected:

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	int MaxInteractionDistance;
};
