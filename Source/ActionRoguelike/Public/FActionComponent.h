// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "FActionEffect.h"
#include "FAction_Interactable.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "FActionComponent.generated.h"


class UFAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UFActionComponent*, OwningComponent, UFAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UFActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFActionComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = Action)
	void AddAction(AActor* Instigator, TSubclassOf<UFAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = Action)
	void RemoveAction(UFActionEffect* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = Action)
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = Action)
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = Action)
	UFAction* GetAction(TSubclassOf<UFAction> ActioType);
	
	UPROPERTY(BlueprintAssignable, Category = Actions)
	FOnActionStateChanged OnActionStarted;
	
	UPROPERTY(BlueprintAssignable, Category = Actions)
	FOnActionStateChanged OnActionStopped;
	

protected:
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);
	
	UPROPERTY(EditAnywhere, Category = Actions)
	TArray<TSubclassOf<UFAction>> DefaultActions;
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<UFAction*> Actions;

	virtual void BeginPlay() override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags);
};
