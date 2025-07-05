// Fill out your copyright notice in the Description page of Project Settings.


#include "FTeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AFTeleportProjectile::AFTeleportProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement->InitialSpeed = 3000.0f;
	ParticlePortalStart = CreateDefaultSubobject<UParticleSystemComponent>("Portal Start");
	ParticlePortalStart->SetupAttachment(RootComponent);
	ParticlePortalStart->bAutoActivate = false;
	
	ParticlePortalEnd = CreateDefaultSubobject<UParticleSystemComponent>("Portal End");
	ParticlePortalEnd->bAutoActivate = false;
	ParticlePortalEnd->SetupAttachment(RootComponent);
}

void AFTeleportProjectile::OnTriggerEffect()
{
	ProjectileMovement->StopMovementImmediately();
	OnNotifyTriggerEffect.Broadcast(this);
	ParticlePortalEnd->Activate();
	ParticleComponent->DeactivateImmediate();
	ParticlePortalEnd->OnSystemFinished.AddUniqueDynamic(this, &AFTeleportProjectile::OnPortalEndParticleStoppedPlayingDelegate);
}

void AFTeleportProjectile::OnPortalEndParticleStoppedPlayingDelegate(UParticleSystemComponent* _)
{
	Destroy();
}

// Called when the game starts or when spawned
void AFTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	ParticlePortalStart->Activate();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFTeleportProjectile::OnTriggerEffect, 0.2f, false);
}

// Called every frame
void AFTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

