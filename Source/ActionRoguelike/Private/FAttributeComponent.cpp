// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#include "FAttributeComponent.h"
#include "FGameModeBase.h"
#include "Net/UnrealNetwork.h"

UFAttributeComponent::UFAttributeComponent()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
	Rage = 0.0f;
	MaxRage = 100.0f;
	SetIsReplicatedByDefault(true);
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

float UFAttributeComponent::GetRage() const
{
	return Rage;
}

void UFAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChange.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void UFAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChange.Broadcast(InstigatorActor, this, NewRage, Delta);
}
void UFAttributeComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFAttributeComponent, Health);
	DOREPLIFETIME(UFAttributeComponent, Rage);
	DOREPLIFETIME(UFAttributeComponent, MaxHealth);
	DOREPLIFETIME(UFAttributeComponent, MaxRage);
}

bool UFAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged()) return false;
	
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	
	const float ActualDelta = Health - OldHealth;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	if (ActualDelta != 0.0f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}
	
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		if (AFGameModeBase* GameMode = Cast<AFGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	return ActualDelta != 0.0f;
}

bool UFAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	if (Delta == 0.0f) return false;
	float OldRage = Rage;
	if (Delta > 0.0f)
	{
		if (MaxRage - Rage == 0.0f)
		{
			return false;
		}
	}
	else
	{
		if (Rage + Delta < 0.0f) return false;
	}
	Rage = FMath::Clamp(Rage + Delta, 0.0f, MaxRage);
	float RealDelta = Rage - OldRage;
	MulticastRageChanged(InstigatorActor, Rage, RealDelta);
	return true;
}

bool UFAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -MaxHealth);
}