// Fill out your copyright notice in the Description page of Project Settings.
#include "ActionRoguelike/Public/FMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AFMagicProjectile::AFMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;
	
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComponent");
	ParticleComponent->SetupAttachment(SphereComponent);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
}

void AFMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (APawn* ProjectileInstigator = GetInstigator())
	{
		SphereComponent->IgnoreActorWhenMoving(ProjectileInstigator, true);
		ProjectileInstigator->MoveIgnoreActorAdd(this);
	}
}
	

// Called when the game starts or when spawned
void AFMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

