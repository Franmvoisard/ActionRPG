// Public Domain - 2025 Franco Voisard.
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.


#include "FActionComponent.h"

#include "DebugCVar.h"
#include "FAction.h"
#include "ActionRoguelike/DebugUtils.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UFActionComponent::UFActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UFActionComponent::AddAction(AActor* Instigator, TSubclassOf<UFAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to add a null action class"));
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to add an action of type [%s] on a client actor"), *GetNameSafe(ActionClass));
		return;
	}
	UFAction* NewAction = NewObject<UFAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);
		if (NewAction->bAutoStart && NewAction->CanStart(Instigator))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UFActionComponent::RemoveAction(UFActionEffect* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	
	Actions.Remove(ActionToRemove);
}

bool UFActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UFAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				DebugUtils::ActionSystem::DrawScreenText(FString::Printf(TEXT("Action %s cannot be started"), *Action->GetName()));
				continue;
			}
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UFActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UFAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);	
				}
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

UFAction* UFActionComponent::GetAction(TSubclassOf<UFAction> ActionType)
{
	for (UFAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionType))
		{
			return Action;
		}
	}
	return nullptr;
}

void UFActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UFActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

void UFActionComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UFAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UFActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<UFAction*> ActionsCopy = Actions;
	for (UFAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}
	Super::EndPlay(EndPlayReason);
}

void UFActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFActionComponent, Actions);
}

bool UFActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
                                            FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UFAction* Action : Actions)
	{
		if (Action)
		{
			if (Channel->ReplicateSubobject(Action, *Bunch, *RepFlags))
			{
				bWroteSomething = true;
			}
		}
	}
	return bWroteSomething;
}
