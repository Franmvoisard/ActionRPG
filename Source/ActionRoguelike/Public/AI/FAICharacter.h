// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "FAttributeComponent.h"
#include "GameFramework/Character.h"
#include "FAICharacter.generated.h"

class UPawnSensingComponent;
UCLASS()
class ACTIONROGUELIKE_API AFAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	UFAttributeComponent* AttributeComponent;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UFAttributeComponent* OwnerAttributeComponent, float NewHealth, float Delta);
	virtual void PostInitializeComponents() override;
};
	
