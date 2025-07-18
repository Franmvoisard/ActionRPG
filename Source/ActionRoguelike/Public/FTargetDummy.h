// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FTargetDummy.generated.h"

class UFFlashOnHitComponent;
class UFAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AFTargetDummy : public AActor
{
	GENERATED_BODY()

	
public:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UFAttributeComponent* OwningComponent, float NewHealth, float Delta);
	AFTargetDummy();

	UPROPERTY(EditDefaultsOnly)
	UFFlashOnHitComponent* FlashOnHitComponent;
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;
	

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UFAttributeComponent* AttributeComponent;
};
