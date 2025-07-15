// Fill out your copyright notice in the Description page of Project Settings.


#include "FTargetDummy.h"
#include "FAttributeComponent.h"
#include "ActionRoguelike/FFlashOnHitComponent.h"
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

void AFTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChange.AddDynamic(this, &AFTargetDummy::OnHealthChanged);
}

