// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "AI/FBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UFBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check Distance between AI Pawn and target actor

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		if (AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName)))
		{
			AAIController* OwnerAIController = OwnerComp.GetAIOwner();
			if (ensure(OwnerAIController))
			{
				APawn* AIPawn = OwnerAIController->GetPawn();
				if (ensure(AIPawn))
				{
					const float DistanceToTarget = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					const bool bWithinRange = DistanceToTarget < AttackRange;
					bool bHasSightOfTarget = false;
					if (bWithinRange)
					{
						bHasSightOfTarget = OwnerAIController->LineOfSightTo(TargetActor);
					}

					BlackboardComp->SetValueAsBool(IsInRangeKey.SelectedKeyName, bWithinRange && bHasSightOfTarget);
				}
			}
		}
	}
}
