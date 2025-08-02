// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FBTTask_RangedAttack.generated.h"

class AFProjectileBase;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UFBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = AI)
	TSubclassOf<AFProjectileBase> ProjectileClass;
};
