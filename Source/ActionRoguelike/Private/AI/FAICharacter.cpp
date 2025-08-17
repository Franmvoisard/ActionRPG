// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "AI/FAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "FAttributeComponent.h"
#include "FWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

AFAICharacter::AFAICharacter()
{
 	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing Component");
	AttributeComponent = CreateDefaultSubobject<UFAttributeComponent>("Attribute Component");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void AFAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddUniqueDynamic(this, &AFAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChange.AddDynamic(this, &AFAICharacter::OnHealthChanged);
}

void AFAICharacter::SetTargetActor(AActor* TargetActor)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetActor);
	}
}

void AFAICharacter::OnHealthChanged(AActor* InstigatorActor, UFAttributeComponent* OwnerAttributeComponent, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UFWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
		
		if (NewHealth <= 0)
		{
			if (AAIController* AIController = Cast<AAIController>(GetController()))
			{
				AIController->GetBrainComponent()->StopLogic("Killed");
			}
			
			//Ragdoll and die
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			SetLifeSpan(10.0f);
			OnKilled.Broadcast(this, InstigatorActor);
		}
	}
}

void AFAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);	
	DrawDebugString(GetWorld(),GetActorLocation(), "Player seen", nullptr, FColor::Red, 2.0f);
}


