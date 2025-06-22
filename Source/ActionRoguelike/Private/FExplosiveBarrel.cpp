// Fill out your copyright notice in the Description page of Project Settings.


#include "FExplosiveBarrel.h"

#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AFExplosiveBarrel::AFExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BarrelStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Capsule Component"));
	RootComponent = BarrelStaticMesh;
	BarrelStaticMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

	InitializeRadialForceComponent();
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticle"));
}

void AFExplosiveBarrel::InitializeRadialForceComponent()
{
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force Component"));
	RadialForceComponent->SetupAttachment(GetRootComponent());
	RadialForceComponent->Radius = 500.0f;
	RadialForceComponent->ImpulseStrength = 2000.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->bIgnoreOwningActor = false;
}

void AFExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	BarrelStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AFExplosiveBarrel::OnBarrelBeginOverlap);
}

void AFExplosiveBarrel::OnBarrelBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped"));
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("ExplosiveBarrel overlapped with %s"), *OtherActor->GetName());
		Explode();
	}
}

void AFExplosiveBarrel::Explode()
{
	RadialForceComponent->FireImpulse();
	ParticleComponent->Activate();
}

// Called every frame
void AFExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

