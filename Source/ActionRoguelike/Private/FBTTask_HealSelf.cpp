// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FBTTask_HealSelf.h"

#include "AIController.h"
#include "FAttributeComponent.h"

EBTNodeResult::Type UFBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AActor* OwnerActor = AIController->GetPawn())
		{
			if (UFAttributeComponent* AttributeComponent = UFAttributeComponent::GetAttributes(OwnerActor))
			{
				AttributeComponent->ApplyHealthChange(OwnerActor, AttributeComponent->GetMaxHealth());
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
