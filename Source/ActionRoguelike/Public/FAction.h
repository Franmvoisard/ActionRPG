// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FAction.generated.h"

class UFActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY();
public:
	FActionRepData()
		: bIsRunning(false)
		, Instigator(nullptr)
	{}
	
	UPROPERTY()
	bool bIsRunning;
	
	UPROPERTY()
	AActor* Instigator;

};
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UFAction : public UObject
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnRep_RepData();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= UI)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UFActionComponent* GetOwningComponent() const;

	UPROPERTY(ReplicatedUsing = OnRep_RepData)
	FActionRepData RepData;

	UPROPERTY(Replicated)
	UFActionComponent* OwnerActionComponent;
	
	UPROPERTY(Replicated)
	float TimeStarted;

public:
	UFAction();

	void Initialize(UFActionComponent* ActionComponent);
	
	UPROPERTY(EditAnywhere, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	virtual UWorld* GetWorld() const override;
	virtual bool IsSupportedForNetworking() const override;
};
