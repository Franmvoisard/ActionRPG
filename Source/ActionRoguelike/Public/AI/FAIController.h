// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AFAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree")
	UBehaviorTree* BehaviorTree;
};
