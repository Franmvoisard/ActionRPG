// Public Domain - 2025 Franco Voisard.
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.


#include "FThornActionEffect.h"

#include "FActionComponent.h"
#include "FAttributeComponent.h"
#include "FGameplayFunctionLibrary.h"

UFThornActionEffect::UFThornActionEffect()
{
	DamageReflectionRatio = 0.5f;
	Duration = 0.0f;
	Period = 0.0f;
	bAutoStart = true;
	ActionName = "Thorn";
}

void UFThornActionEffect::ReflectDamage(AActor* InstigatorActor, UFAttributeComponent* Owner, float NewValue, float Delta)
{
	//Only reflect if the health change is damage
	if (Delta >= 0.0f) return;
	AActor* OwningActor = GetOwningComponent()->GetOwner();
	
	if (OwningActor && OwningActor != InstigatorActor)
	{
		float ReflectedAmount = FMath::RoundToInt(abs(Delta * DamageReflectionRatio));
		
		if (ReflectedAmount == 0.0f)
		{
			return;
		}
		
		UFGameplayFunctionLibrary::ApplyDamage(InstigatorActor, OwningActor, ReflectedAmount);
	}
}

void UFThornActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	AActor* OwnerActor = GetOwningComponent()->GetOwner();
	if (UFAttributeComponent* AttributeComponent = UFAttributeComponent::GetAttributes(OwnerActor))
	{
		AttributeComponent->OnHealthChange.AddDynamic(this, &UFThornActionEffect::ReflectDamage);
	}
}

void UFThornActionEffect::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	AActor* OwnerActor = GetOwningComponent()->GetOwner();
	if (UFAttributeComponent* AttributeComponent = UFAttributeComponent::GetAttributes(OwnerActor))
	{
		AttributeComponent->OnHealthChange.RemoveDynamic(this, &UFThornActionEffect::ReflectDamage);
	}
}