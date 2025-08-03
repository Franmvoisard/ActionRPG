// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#pragma once

#include "CoreMinimal.h"
#include "FGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "FHealthPotion.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API AFHealthPotion : public AActor, public IFGameplayInterface
{
	GENERATED_BODY()
	FTimerHandle Timer_ResetInteraction;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	FLinearColor InteractableColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor NonInteractableColor;
	
public:	
	AFHealthPotion();

protected:
	virtual void BeginPlay() override;
	void SetInteractionState(bool IsActive);
	void ResetInteractionTimer_Elapsed();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	bool IsInteractable;

private:
	bool HealInstigator(const APawn* InstigatorToHeal);
};
