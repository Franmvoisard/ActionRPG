// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FActionComponent.h"

#include "FAction.h"

// Sets default values for this component's properties
UFActionComponent::UFActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UFActionComponent::AddAction(TSubclassOf<UFAction> ActionClass)
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
	}
}

bool UFActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UFAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
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
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

