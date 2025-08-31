// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FAction.h"

UFAction::UFAction()
{
}

void UFAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Running Action: %s"), *GetNameSafe(this));
	UFActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
	
}

void UFAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Stopped Action: %s"), *GetNameSafe(this));
	UFActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
}

UWorld* UFAction::GetWorld() const
{
	if (UActorComponent* ActorComponent = Cast<UActorComponent> (GetOuter()))
	{
		return ActorComponent->GetWorld();
	}
	return nullptr;
}

UFActionComponent* UFAction::GetOwningComponent() const
{
	return Cast<UFActionComponent>(GetOuter());
}