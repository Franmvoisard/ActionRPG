// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#pragma once

#include "CoreMinimal.h"
#include "FGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "FCooldownInteractable.generated.h"

class USphereComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API AFCooldownInteractable : public AActor, public IFGameplayInterface
{
	GENERATED_BODY()

	
public:	
	AFCooldownInteractable();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly)
	float Cooldown;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	FLinearColor InteractableColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor NonInteractableColor;
	FTimerHandle Timer_ResetInteraction;
	bool IsInteractable;
	
	void SetInteractionState(bool IsActive);
	void ResetInteractionTimer_Elapsed();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
