// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "FActionComponent.h"
#include "GameplayTagContainer.h"
#include "FAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UFAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UFActionComponent* GetOwningComponent() const;
	bool bIsRunning = false;

public:
	UFAction();

	UPROPERTY(EditAnywhere, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;
	UWorld* GetWorld() const override;
};
