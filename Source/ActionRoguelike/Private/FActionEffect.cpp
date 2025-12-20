// Public Domain - 2025 Franco Voisard.
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.

#include "FActionEffect.h"
#include "FActionComponent.h"
#include "GameFramework/GameStateBase.h"

UFActionEffect::UFActionEffect()
{
	bAutoStart = true;
}

void UFActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	TWeakObjectPtr<AActor> WeakInstigator = Instigator;
	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindLambda([this, WeakInstigator]()
		{
			if (WeakInstigator.IsValid())
			{
				StopAction(WeakInstigator.Get());
			}
		});

		GetWorld()->GetTimerManager().SetTimer(DurationTimerHandle, Delegate, Duration, false);
	}
	
	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindLambda([this, WeakInstigator]()
		{
			if (WeakInstigator.IsValid())
			{
				ExecutePeriodicEffect(WeakInstigator.Get());
			}
		});
		
		GetWorld()->GetTimerManager().SetTimer(PeriodTimerHandle, Delegate, Period, true);
	}
}

void UFActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{

	
}

void UFActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodTimerHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);	
	}
	
	Super::StopAction_Implementation(Instigator);
	GetWorld()->GetTimerManager().ClearTimer(PeriodTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationTimerHandle);

	if (UFActionComponent* ActionComponent = GetOwningComponent())
	{
		ActionComponent->RemoveAction(this);
	}
}

float UFActionEffect::GetTimeRemaining() const
{
	if (Duration == 0.0f) return 0;
	if (AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>())
	{
		const float EndTime = TimeStarted + Duration;
		return EndTime - GameState->GetServerWorldTimeSeconds();
	}
	return Duration;
}