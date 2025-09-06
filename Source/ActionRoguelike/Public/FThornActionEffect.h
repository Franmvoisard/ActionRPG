// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "FActionEffect.h"
#include "FAttributeComponent.h"
#include "FThornActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UFThornActionEffect : public UFActionEffect
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float DamageReflectionRatio;

	UFUNCTION()
	void ReflectDamage(AActor* InstigatorActor, UFAttributeComponent* Owner, float NewValue, float Delta);
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
public:
	UFThornActionEffect();
};
