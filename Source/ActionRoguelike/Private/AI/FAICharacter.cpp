// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "AI/FAICharacter.h"

#include "AIController.h"
#include "FAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AFAICharacter::AFAICharacter()
{
 	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing Component");
	AttributeComponent = CreateDefaultSubobject<UFAttributeComponent>("Attribute Component");
}

// Called when the game starts or when spawned
void AFAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddUniqueDynamic(this, &AFAICharacter::OnPawnSeen);
}

void AFAICharacter::OnPawnSeen(APawn* Pawn)
{
	if(AAIController* AIController = Cast<AAIController>(GetController()))
	{
		UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
		BlackboardComponent->SetValueAsObject("TargetActor", Pawn);
		DrawDebugString(GetWorld(),GetActorLocation(), "Player seen", nullptr, FColor::Red, 2.0f);
	}
}


