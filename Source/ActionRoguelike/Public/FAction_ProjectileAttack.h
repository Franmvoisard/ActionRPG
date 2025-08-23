// Public Domain - 2025 Franco Voisard.
// This code is provided for skill and knowledge demo purposes.
// No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "FAction.h"
#include "FAction_ProjectileAttack.generated.h"

class AFProjectileBase;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UFAction_ProjectileAttack : public UFAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Abilities)
	TSubclassOf<AFProjectileBase> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = VFX)
	UParticleSystem* AbilitySpawnParticle;
	
	UPROPERTY(EditAnywhere, Category = Attack);
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float ProjectileTraceSphereRadius;
	
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	int ProjectileMaxTraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float AttackAnimationDelay;

public:
	UFAction_ProjectileAttack();
	virtual void StartAction_Implementation(AActor* Instigator) override;

	UFUNCTION()
	virtual void ProjectileAttack_TimeElapsed(ACharacter* InstigatorCharacter);
};
