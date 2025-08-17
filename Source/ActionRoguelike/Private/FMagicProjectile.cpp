// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#include "FMagicProjectile.h"
#include "FAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AFMagicProjectile::AFMagicProjectile()
{
	ProjectileDamage = 10.0f;
	ImpactSoundFX = nullptr;
}

// Called when the game starts or when spawned

void AFMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFMagicProjectile::OnActorBeginOverlap_Implementation);
}

void AFMagicProjectile::Explode_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSoundFX, GetActorLocation());
	Super::Explode_Implementation();
}

void AFMagicProjectile::OnActorBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped"));
	
	if (OtherActor != GetInstigator())
	{
		if (UFAttributeComponent* OtherActorAttributeComponent = OtherActor->GetComponentByClass<UFAttributeComponent>())
		{
			OtherActorAttributeComponent->ApplyHealthChange(GetInstigator(), -ProjectileDamage);
			Explode_Implementation();
		}
	}
}
