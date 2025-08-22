// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FActionComponent.generated.h"


class UFAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UFActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFActionComponent();

	UFUNCTION(BlueprintCallable, Category = Action)
	void AddAction(TSubclassOf<UFAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = Action)
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = Action)
	bool StopActionByName(AActor* Instigator, FName ActionName);
	
protected:
	UPROPERTY()
	TArray<UFAction*> Actions;
};
