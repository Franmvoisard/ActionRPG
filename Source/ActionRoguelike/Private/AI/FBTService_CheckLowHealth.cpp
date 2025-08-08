// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#include "AI/FBTService_CheckLowHealth.h"
#include "BehaviorTree/BlackboardComponent.h"

void UFBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (AAIController* OwnerActorController = OwnerComp.GetAIOwner())
	{
		if(AActor* OwnerActor = OwnerActorController->GetPawn())
		{
			if (UFAttributeComponent* HealthAttribute = OwnerActor->FindComponentByClass<UFAttributeComponent>())
			{
				const bool bIsLowHealth = HealthAttribute->GetHealth() / HealthAttribute->GetMaxHealth() < NormalizedLowHealthThreshold;
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsLowHealthKey.SelectedKeyName, bIsLowHealth);
			}
		}
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("No AAIController found on %s"), *OwnerComp.GetAIOwner()->GetName());
	}
}
