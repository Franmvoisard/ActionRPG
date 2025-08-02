// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "AI/FBTTask_RangedAttack.h"

#include "AIController.h"
#include "FProjectileBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UFBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(AIController->GetPawn());
		if (OwnerCharacter == nullptr) return EBTNodeResult::Failed;

		FVector MuzzleLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Muzzle_01");
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr) return EBTNodeResult::Failed;
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Owner = OwnerCharacter;
		SpawnParameters.Instigator = OwnerCharacter;
		AActor* Projectile = GetWorld()->SpawnActor<AFProjectileBase>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParameters);
		return Projectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
