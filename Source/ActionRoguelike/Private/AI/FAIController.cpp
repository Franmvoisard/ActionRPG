// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#include "AI/FAIController.h"

void AFAIController::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(BehaviorTree, TEXT("BehaviorTree is not set, assign Behavior Tree in the AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
 