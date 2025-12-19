// Public Domain - 2025 Franco Voisard. 
// This code is provided for skill and knowledge demo purposes. 
// No rights reserved. Use freely.


#include "FAction.h"

#include "FActionComponent.h"
#include "ActionRoguelike/DebugUtils.h"
#include "Net/UnrealNetwork.h"

UFAction::UFAction()
{
}

void UFAction::Initialize(UFActionComponent* ActionComponent)
{
	OwnerActionComponent =ActionComponent;
}

bool UFAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning()) return false;
	
	if (OwnerActionComponent->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void UFAction::StartAction_Implementation(AActor* Instigator)
{
	DebugUtils::ActionSystem::DebugLog(LogTemp, ELogVerbosity::Type::Log, (TEXT("Running Action: %s"), *GetNameSafe(this)));
	
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
	OwnerActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
	
	GetOwningComponent()->OnActionStarted.Broadcast(OwnerActionComponent, this);
}

void UFAction::StopAction_Implementation(AActor* Instigator)
{
	DebugUtils::ActionSystem::DebugLog(LogTemp, ELogVerbosity::Type::Log,(TEXT("Stopped Action: %s"), *GetNameSafe(this)));
	
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
	OwnerActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
	GetOwningComponent()->OnActionStopped.Broadcast(OwnerActionComponent, this);
}

bool UFAction::IsRunning() const
{
	return RepData.bIsRunning;
}

UWorld* UFAction::GetWorld() const
{
	if (AActor* ActorComponent = Cast<AActor> (GetOuter()))
	{
		return ActorComponent->GetWorld();
	}
	return nullptr;
}

void UFAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

UFActionComponent* UFAction::GetOwningComponent() const
{
	return Cast<UFActionComponent>(GetOuter());
}

void UFAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFAction, RepData);
	DOREPLIFETIME(UFAction, OwnerActionComponent);
}

bool UFAction::IsSupportedForNetworking() const
{
	return true;
}