// Fill out your copyright notice in the Description page of Project Settings.
#include "ActionRoguelike/Public/FProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
DEFINE_LOG_CATEGORY(Projectiles);
AFProjectileBase::AFProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	
	RootComponent = SphereComponent;
	
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComponent");
	ParticleComponent->SetupAttachment(SphereComponent);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	ProjectileMovement->InitialSpeed = 8000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
}

void AFProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	UE_LOGFMT(Projectiles, Display, "Projectile hit with actor `{ActorName}` and component `{ComponentName}`, instigator was `{Instigator}`", *OtherActor->GetName(), *OtherComp->GetName(), GetInstigator() != nullptr ? *GetInstigator()->GetName() : TEXT("null"));
	if (GetInstigator() != OtherActor)
	{
		Explode();
	}
}

void AFProjectileBase::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{ 
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		Destroy();
	}
}

void AFProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (APawn* ProjectileInstigator = GetInstigator())
	{
		SphereComponent->IgnoreActorWhenMoving(ProjectileInstigator, true);
		ProjectileInstigator->MoveIgnoreActorAdd(this);
	}
}


void AFProjectileBase::PlaySpawnSound() const
{
	if (SpawnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no SpawnSound set"), *GetNameSafe(this));
	}
}

// Called when the game starts or when spawned
void AFProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	PlaySpawnSound();
	SphereComponent->OnComponentHit.AddDynamic(this, &AFProjectileBase::OnActorHit);
}

void AFProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SphereComponent->OnComponentHit.RemoveDynamic(this, &AFProjectileBase::OnActorHit);
}

// Called every frame
void AFProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}