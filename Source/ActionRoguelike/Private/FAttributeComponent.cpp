// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#include "FAttributeComponent.h"

#include "FGameModeBase.h"

UFAttributeComponent::UFAttributeComponent()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
}

UFAttributeComponent* UFAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<UFAttributeComponent>();
	}
	return nullptr;
}

bool UFAttributeComponent::IsActorAlive(AActor* FromActor)
{
	if (UFAttributeComponent* ActorAttributeComponent = GetAttributes(FromActor))
	{
		return ActorAttributeComponent->IsAlive();
	}
	return false;
}

bool UFAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UFAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}

float UFAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UFAttributeComponent::GetHealth() const
{
	return Health;
}

bool UFAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged()) return false;
	
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	
	const float ActualDelta = Health - OldHealth;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	OnHealthChange.Broadcast(InstigatorActor, this, Health, ActualDelta);
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		if (AFGameModeBase* GameMode = Cast<AFGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	return ActualDelta != 0.0f;
}

bool UFAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -MaxHealth);
}
