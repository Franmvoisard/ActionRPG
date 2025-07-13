// Fill out your copyright notice in the Description page of Project Settings.


#include "FTeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AFTeleportProjectile::AFTeleportProjectile()
{
	ProjectileMovement->InitialSpeed = 3000.0f;
	ParticlePortalStart = CreateDefaultSubobject<UParticleSystemComponent>("Portal Start");
	ParticlePortalStart->SetupAttachment(RootComponent);
	ParticlePortalStart->bAutoActivate = false;
	
	ParticlePortalEnd = CreateDefaultSubobject<UParticleSystemComponent>("Portal End");
	ParticlePortalEnd->bAutoActivate = false;
	ParticlePortalEnd->SetupAttachment(RootComponent);
}


void AFTeleportProjectile::OnPortalEndParticleStoppedPlayingDelegate(UParticleSystemComponent* _)
{
	//Destroy();
}

void AFTeleportProjectile::Explode_Implementation()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("EXPLODE"));
	}
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	ParticleComponent->DeactivateSystem();
	ProjectileMovement->StopMovementImmediately();
	SetActorEnableCollision(false);
	ParticlePortalEnd->Activate();
	//ParticlePortalEnd->OnSystemFinished.AddUniqueDynamic(this, &AFTeleportProjectile::OnPortalEndParticleStoppedPlayingDelegate);
	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &AFTeleportProjectile::TeleportInstigator, TeleportDelay);
}

void AFTeleportProjectile::TeleportInstigator()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("START TELEPORT"));
	}
	
	APawn* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), TeleportSound, GetActorLocation());
		Destroy(); 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InstigatorPawn is null"));
	}
}

// Called when the game starts or when spawned
void AFTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	ParticlePortalStart->Activate();
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &AFTeleportProjectile::Explode, DetonateDelay);
}

