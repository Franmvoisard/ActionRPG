// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "AI/FAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AFAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);

	if (APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	}
}
 