// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.

#include "FExplosiveBarrel.h"

#include "FAttributeComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values

AFExplosiveBarrel::AFExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BarrelStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Capsule Component"));
	BarrelStaticMesh->SetSimulatePhysics(true);
	RootComponent = BarrelStaticMesh;
	BarrelStaticMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	BarrelStaticMesh->CanCharacterStepUpOn = ECB_No;
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticle"));
	InitializeRadialForceComponent();
}

void AFExplosiveBarrel::InitializeRadialForceComponent()
{
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force Component"));
	RadialForceComponent->SetupAttachment(GetRootComponent());
	ParticleComponent->SetupAttachment(GetRootComponent());
	RadialForceComponent->Radius = 500.0f;
	RadialForceComponent->ImpulseStrength = 2000.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->bIgnoreOwningActor = false;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void AFExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (HasAuthority())
	{
		BarrelStaticMesh->OnComponentHit.AddDynamic(this, &AFExplosiveBarrel::OnBarrelHit);
	}
}

void AFExplosiveBarrel::OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                   FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		if (UFAttributeComponent* AttributeComponent = UFAttributeComponent::GetAttributes(OtherActor))
		{
			AttributeComponent->ApplyHealthChange(this, -20.0f);	
		}
		UE_LOG(LogTemp, Warning, TEXT("ExplosiveBarrel overlapped with %s"), *OtherActor->GetName());
		Explode();
	}
}

void AFExplosiveBarrel::Explode_Implementation()
{
	Multicast_Explode();
}

void AFExplosiveBarrel::Multicast_Explode_Implementation()
{
	RadialForceComponent->FireImpulse();
	ParticleComponent->Activate();
}