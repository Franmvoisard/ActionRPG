// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "FAIController.h"
#include "BehaviorTree/BTService.h"
#include "FAttributeComponent.h"
#include "FBTService_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UFBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = AI)
	float NormalizedLowHealthThreshold;

	UPROPERTY(EditAnywhere, Category = AI)
	FBlackboardKeySelector IsLowHealthKey;
};
