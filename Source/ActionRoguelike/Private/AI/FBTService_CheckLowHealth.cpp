// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#include "AI/FBTService_CheckLowHealth.h"
#include "BehaviorTree/BlackboardComponent.h"

void UFBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (AActor* OwnerActor = OwnerComp.GetAIOwner())
	{
		if (UFAttributeComponent* HealthAttribute = OwnerActor->FindComponentByClass<UFAttributeComponent>())
		{
			const bool bIsLowHealth = HealthAttribute->GetMaxHealth() / HealthAttribute->GetHealth() < NormalizedLowHealthThreshold;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsLowHealthKey.SelectedKeyName, bIsLowHealth);
		}
	}
}
