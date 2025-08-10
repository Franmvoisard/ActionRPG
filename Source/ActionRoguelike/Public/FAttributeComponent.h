// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChange, AActor*, InstigatorActor, UFAttributeComponent*, Owner, float, NewHealth, float, Delta);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UFAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFAttributeComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;
	bool IsFullHealth() const;
	float GetMaxHealth() const;
	float GetHealth() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;

public:	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnHealthChange OnHealthChange;
};
