// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FAnimInstance.h"
#include "FActionComponent.h"
#include "GameplayTagContainer.h"

void UFAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (AActor* ActorComponent = GetOwningActor())
	{
		ActionComp = Cast<UFActionComponent>(ActorComponent->GetComponentByClass<UFActionComponent>());
	}
}

void UFAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag(FName("Status.Stunned"));
	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
