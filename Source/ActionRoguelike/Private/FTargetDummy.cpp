// Fill out your copyright notice in the Description page of Project Settings.


#include "FTargetDummy.h"
#include "FAttributeComponent.h"

const char* FlashHitParameterName = "TimeToHit";
AFTargetDummy::AFTargetDummy()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshComponent;
	AttributeComponent = CreateDefaultSubobject<UFAttributeComponent>("Attribute Component");
}

void AFTargetDummy::OnHealthChanged(AActor* InstigatorActor, UFAttributeComponent* OwningComponent, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComponent->SetScalarParameterValueOnMaterials(FlashHitParameterName, GetWorld()->TimeSeconds);
	}
}

// Called when the game starts or when spawned
void AFTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChange.AddDynamic(this, &AFTargetDummy::OnHealthChanged);
}

