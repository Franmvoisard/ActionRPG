// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.

#include "FTargetDummy.h"
#include "FAttributeComponent.h"
#include "FFlashOnHitComponent.h"
AFTargetDummy::AFTargetDummy()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshComponent;
	AttributeComponent = CreateDefaultSubobject<UFAttributeComponent>("Attribute Component");
	FlashOnHitComponent = CreateDefaultSubobject<UFFlashOnHitComponent>("Flash On Hit Component");
}

void AFTargetDummy::OnHealthChanged(AActor* InstigatorActor, UFAttributeComponent* OwningComponent, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		FlashOnHitComponent->OnHit(MeshComponent);
	}
}

void AFTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	AttributeComponent->OnHealthChange.AddDynamic(this, &AFTargetDummy::OnHealthChanged);
}

void AFTargetDummy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AttributeComponent->OnHealthChange.RemoveDynamic(this, &AFTargetDummy::OnHealthChanged);
}
