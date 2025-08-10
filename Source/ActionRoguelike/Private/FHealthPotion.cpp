// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.

#include "FHealthPotion.h"
#include "FAttributeComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AFHealthPotion::AFHealthPotion()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshComponent;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);
	IsInteractable = true;
}

// Called when the game starts or when spawned
void AFHealthPotion::BeginPlay()
{
	Super::BeginPlay();
}

void AFHealthPotion::SetInteractionState(bool IsActive)
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

void AFHealthPotion::ResetInteractionTimer_Elapsed()
{
	SetInteractionState(true);
}

void AFHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	ensure(IsValid(InstigatorPawn));
	if (IsInteractable)
	{
		if (HealInstigator(InstigatorPawn))
		{
			GetWorldTimerManager().SetTimer(Timer_ResetInteraction, this, &AFHealthPotion::ResetInteractionTimer_Elapsed,10.0f);
			SetInteractionState(false);
		}
	}	
}

bool AFHealthPotion::HealInstigator(const APawn* InstigatorToHeal)
{
	if (UFAttributeComponent* InstigatorAttributeComponent = InstigatorToHeal->GetComponentByClass<UFAttributeComponent>())
	{
		return InstigatorAttributeComponent->ApplyHealthChange(this, InstigatorAttributeComponent->GetMaxHealth());
	}

	return false;
}

