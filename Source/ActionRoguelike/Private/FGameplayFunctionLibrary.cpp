// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FGameplayFunctionLibrary.h"

#include "FAttributeComponent.h"

bool UFGameplayFunctionLibrary::ApplyDamage(AActor* Target, AActor* DamageInstigator, float DamageAmount)
{
	UFAttributeComponent* TargetAttributeComponent = UFAttributeComponent::GetAttributes(Target);
	if (TargetAttributeComponent)
	{
		return TargetAttributeComponent->ApplyHealthChange(DamageInstigator, -DamageAmount);
	}
	return false;
}

bool UFGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageTarget, AActor* DamageInstigator,
	float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageTarget, DamageInstigator, DamageAmount))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComponent->AddImpulseAtLocation(-HitResult.ImpactNormal * 3000000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
	
}
