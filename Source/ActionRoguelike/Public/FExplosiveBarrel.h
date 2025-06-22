// Fill out your copyright notice in the Description page of Project Settings.

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
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBarrelBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BarrelStaticMesh;
	
	UPROPERTY(EditAnywhere)
	URadialForceComponent* RadialForceComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* ParticleComponent;
	
	void Explode();

public:
	AFExplosiveBarrel();
	virtual void Tick(float DeltaTime) override;
};
