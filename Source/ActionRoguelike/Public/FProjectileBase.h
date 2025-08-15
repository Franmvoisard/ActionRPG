// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

DECLARE_LOG_CATEGORY_EXTERN(Projectiles, Display, All);
UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API AFProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AFProjectileBase();

protected:

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* SpawnSound;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	
	UParticleSystem* ImpactVFX;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystemComponent* ParticleComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> CameraShakeOnImpact;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float ShakeIgnoreAreaRadius;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float ShakeImpactAreaRadius;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();
	void PlaySpawnSound() const;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;
};
