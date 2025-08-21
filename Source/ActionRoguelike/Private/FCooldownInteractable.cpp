// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.

#include "FCooldownInteractable.h"
#include "Components/SphereComponent.h"

// Sets default values
AFCooldownInteractable::AFCooldownInteractable()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshComponent;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);
	IsInteractable = true;
	Cooldown = 10.0f;
}

void AFCooldownInteractable::SetInteractionState(bool IsActive)
{
	IsInteractable = IsActive;
	// Change color material property
	if (DynamicMaterialInstance == nullptr)
	{
		DynamicMaterialInstance = MeshComponent->CreateDynamicMaterialInstance(0, MeshComponent->GetMaterial(0));
		DynamicMaterialInstance->GetVectorParameterValue(FName("Color"), InteractableColor);
	}
	
	DynamicMaterialInstance->SetVectorParameterValue(FName("Color"), IsActive ? InteractableColor : NonInteractableColor);
}

void AFCooldownInteractable::ResetInteractionTimer_Elapsed()
{
	SetInteractionState(true);
}

void AFCooldownInteractable::Interact_Implementation(APawn* InstigatorPawn)
{
}

