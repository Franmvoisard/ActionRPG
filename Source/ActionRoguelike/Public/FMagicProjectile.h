// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#pragma once

#include "CoreMinimal.h"
#include "FProjectileBase.h"
#include "FMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AFMagicProjectile : public AFProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFMagicProjectile();

protected:

	UFUNCTION()
	void OnActorBeginOverlap_Implementation(AActor* OverlappedActor, AActor* OtherActor);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Explode_Implementation() override;
	
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* ImpactSoundFX;

	UPROPERTY(EditDefaultsOnly, Category = Attributes)
	float ProjectileDamage;
};
