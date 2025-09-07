// Public Domain - 2025 Franco Voisard.
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.


#include "FActionComponent.h"

#include "DebugCVar.h"
#include "FAction.h"

UFActionComponent::UFActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFActionComponent::AddAction(AActor* Instigator, TSubclassOf<UFAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to add a null action class"));
		return;
	}
	UFAction* NewAction = NewObject<UFAction>(this, ActionClass);
	if (ensure(NewAction))
	{
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
				DEBUG_ONLY(
					if (DebugCVar::IsActionsDebugEnabled())
					{
						FString ErrorMessage = FString::Printf(TEXT("Action %s cannot be started"), *Action->GetName());
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, ErrorMessage);
					}
				)
				continue;
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

void UFActionComponent::BeginPlay()
{
	Super::BeginPlay();
	for (TSubclassOf<UFAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}
