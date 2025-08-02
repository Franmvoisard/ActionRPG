// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UFBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float AttackRange;

	UPROPERTY(EditAnywhere, Category = AI)
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = AI)
	FBlackboardKeySelector IsInRangeKey;
};
