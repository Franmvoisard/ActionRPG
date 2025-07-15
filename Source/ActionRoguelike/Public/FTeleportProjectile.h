// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FProjectileBase.h"
#include "FTeleportProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AFTeleportProjectile : public AFProjectileBase
{
	GENERATED_BODY()

public:
	AFTeleportProjectile();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	USoundBase* TeleportSound;
	
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticlePortalStart;
	
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticlePortalEnd;
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;
	virtual void Explode_Implementation() override;

	void TeleportInstigator();
	FTimerHandle TimerHandle_DelayedDetonate;
		
	virtual void BeginPlay() override;
};
