// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FAction.h"

#include "DebugCVar.h"
#include "FActionComponent.h"

UFAction::UFAction()
{
}

bool UFAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning()) return false;
	
	UFActionComponent* ActionComponent = GetOwningComponent();
	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void UFAction::StartAction_Implementation(AActor* Instigator)
{
	DEBUG_ONLY
	(
		if (DebugCVar::IsActionsDebugEnabled())
		{
			UE_LOG(LogTemp, Warning, TEXT("Running Action: %s"), *GetNameSafe(this));
		}
	)
	bIsRunning = true;
	UFActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
}

void UFAction::StopAction_Implementation(AActor* Instigator)
{
	ensureAlways(bIsRunning);
	DEBUG_ONLY
	(
		if (DebugCVar::IsActionsDebugEnabled())
		{
			UE_LOG(LogTemp, Warning, TEXT("Stopped Action: %s"), *GetNameSafe(this));
		}
	)
	bIsRunning = false;
	UFActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
}

bool UFAction::IsRunning() const
{
	return bIsRunning;
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