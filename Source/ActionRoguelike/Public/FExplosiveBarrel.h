// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "FExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API AFExplosiveBarrel : public AActor
{
	GENERATED_BODY()

private:
	void InitializeRadialForceComponent();
	
protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BarrelStaticMesh;
	
	UPROPERTY(EditAnywhere)
	URadialForceComponent* RadialForceComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* ParticleComponent;

	UFUNCTION(BlueprintNativeEvent)
	void Explode();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_Explode();

public:
	AFExplosiveBarrel();
};
