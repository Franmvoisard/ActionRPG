// Fill out your copyright notice in the Description page of Project Settings.


#include "FTeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AFTeleportProjectile::AFTeleportProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement->InitialSpeed = 3000.0f;
}

void AFTeleportProjectile::OnTriggerEffect()
{
	ProjectileMovement->StopMovementImmediately();
	OnNotifyTriggerEffect.Broadcast(this);
}

// Called when the game starts or when spawned
void AFTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFTeleportProjectile::OnTriggerEffect, 0.2f, false);
}

// Called every frame
void AFTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

