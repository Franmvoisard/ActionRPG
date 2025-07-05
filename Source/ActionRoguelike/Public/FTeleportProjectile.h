// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FProjectileBase.h"
#include "FTeleportProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AFTeleportProjectile : public AFProjectileBase
{
	GENERATED_BODY()

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeleportProjectile, AFTeleportProjectile*, TeleportProjectile);

public:
	AFTeleportProjectile();

	UPROPERTY(BlueprintAssignable)
	FOnTeleportProjectile OnNotifyTriggerEffect;
	void OnTriggerEffect();

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticlePortalStart;
	
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticlePortalEnd;

	UFUNCTION()
	void OnPortalEndParticleStoppedPlayingDelegate(UParticleSystemComponent* ParticleSystem);
protected:
	FTimerHandle TimerHandle;
		
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
