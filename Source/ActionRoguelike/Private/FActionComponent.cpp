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
				FString ErrorMessage = FString::Printf(TEXT("Action %s cannot be started"), *Action->GetName());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, ErrorMessage);
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

void UFActionComponent::BeginPlay()
{
	Super::BeginPlay();
	for (TSubclassOf<UFAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UFActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FString DebugMessage = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMessage);
}

